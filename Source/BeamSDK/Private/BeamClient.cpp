#include "BeamClient.h"

#include "BeamEnvironment.h"
#include "BeamOperationSigningBy.h"
#include "BeamResult.h"
#include "BeamSession.h"
#include "BeamSaveGameStorage.h"
#include "BeamUtils.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "GenericPlatform/GenericPlatformProcess.h"

DEFINE_LOG_CATEGORY(LogBeamClient);

// NOTE: This can be removed if needed for absolute performance.
UE_DISABLE_OPTIMIZATION

UBeamClient::UBeamClient()
{
	SetEnvironment(EBeamEnvironment::Beta);
}

FString UBeamClient::LaunchURL(const FString& Url)
{
	FString Params, LaunchUrlError;
	FPlatformProcess::LaunchURL(*Url, *Params, &LaunchUrlError);
	return LaunchUrlError;
}

//
// --- Config ---
//

UBeamClient* UBeamClient::SetBeamApiKey(const FString& InPublishableApiKey)
{
	BeamApiKey = InPublishableApiKey;
	return this;
}


UBeamClient* UBeamClient::SetEnvironment(EBeamEnvironment InEnvironment)
{
	FString apiUrl;
	switch (InEnvironment)
	{
	case EBeamEnvironment::Mainnet:
	case EBeamEnvironment::Production:
		apiUrl = "https://api.onbeam.com";
		break;
	default:
		apiUrl = "https://api.beta.onbeam.com";
		break;
	}

	BeamApiUrl = apiUrl;

	// Update the APIs
	ConfigureApi(AssetsApi);
	ConfigureApi(ExchangeApi);
	ConfigureApi(HealthApi);
	ConfigureApi(MarketplaceApi);
	ConfigureApi(SessionsApi);
	ConfigureApi(TransactionsApi);
	ConfigureApi(UsersApi);
	ConfigureApi(OperationApi);
	ConfigureApi(ConnectorApi);
	ConfigureApi(RampApi);
	ConfigureApi(ContractApi);

	return this;
}


UBeamClient* UBeamClient::SetStorage(IBeamStorageInterface* InStorage)
{
	Storage = InStorage;
	return this;
}


UBeamClient* UBeamClient::SetDebugLogging(bool InEnable)
{
	DebugLog = InEnable;
	return this;
}


//
// ----- Async API Calls -----
//


TFuture<BeamConnectionResult> UBeamClient::ConnectUserToGameAsync(FString entityId,
                                                                  int32 chainId,
                                                                  int32 secondsTimeout,
                                                                  TOptional<
	                                                                  PlayerClientCreateConnectionRequestInput::AuthProviderEnum>
                                                                  authProvider,
                                                                  TSharedPtr<FBeamCancellationToken>*
                                                                  OutCancellationToken)
{
	// Track whether this was called from the game thread.
	bool calledFromGameThread = IsInGameThread();

	const auto Promise = MakeShared<TPromise<BeamConnectionResult>, ESPMode::ThreadSafe>();

	// Run on another thread so we can use concepts like sleep() when retrying requests without blocking the game thread
	auto resultFuture = Async(EAsyncExecution::Thread,
	                          [&, Promise, entityId, chainId, authProvider, secondsTimeout, OutCancellationToken]
	                          {
		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Retrieving connection request"));

		                          PlayerClientConnectorApi::CreateConnectionRequestRequest request;
		                          request.PlayerClientCreateConnectionRequestInput.EntityId = entityId;

		                          if (authProvider.IsSet())
		                          {
			                          request.PlayerClientCreateConnectionRequestInput.AuthProvider = authProvider;
		                          }

		                          const auto resPromise = MakeShared<
			                          TPromise<PlayerClientConnectorApi::CreateConnectionRequestResponse>,
			                          ESPMode::ThreadSafe>();
		                          auto resFuture = resPromise->GetFuture();
		                          auto httpReq = ConnectorApi->CreateConnectionRequest(request,
			                          PlayerClientConnectorApi::FCreateConnectionRequestDelegate::CreateLambda(
				                          [&, resPromise](
				                          const PlayerClientConnectorApi::CreateConnectionRequestResponse& res)
				                          {
					                          resPromise->SetValue(res);
				                          }));

		                          auto res = resFuture.Get();
		                          CreateConnectionRequestResponse connRequest = res.Content;

		                          if (connRequest.Status == CreateConnectionRequestStatusEnum::Error)
		                          {
			                          const auto httpResponse = res.GetHttpResponse();
			                          FString errorBody = TEXT("No response body or invalid response.");
			                          if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
			                          {
				                          errorBody = httpResponse->GetContentAsString();
			                          }
			                          return BeamConnectionResult(EBeamResultType::Error, errorBody);
		                          }

		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Opening %s"), *connRequest.Url);

		                          // open browser to connect user
		                          LaunchURL(connRequest.Url);

		                          PlayerClientConnectorApi::GetConnectionRequestRequest gcRequest;
		                          gcRequest.RequestId = connRequest.Id;

		                          auto actionToPerform = [&, gcRequest
			                          ]() -> TFuture<PlayerClientConnectorApi::GetConnectionRequestResponse>
		                          {
			                          const auto gcPromise = MakeShared<
				                          TPromise<PlayerClientConnectorApi::GetConnectionRequestResponse>,
				                          ESPMode::ThreadSafe>();
			                          ConnectorApi->GetConnectionRequest(gcRequest,
			                                                             PlayerClientConnectorApi::FGetConnectionRequestDelegate::CreateLambda(
				                                                             [&, gcPromise](
				                                                             const
				                                                             PlayerClientConnectorApi::GetConnectionRequestResponse
				                                                             & res)
				                                                             {
					                                                             FString resBody = res.GetHttpResponse()
						                                                             ->GetContentAsString();
					                                                             UE_CLOG(DebugLog, LogBeamClient, Log,
						                                                             TEXT(
							                                                             "GetConnectionRequest: response=%s"
						                                                             ), *resBody);
					                                                             gcPromise->SetValue(res);
				                                                             }));
			                          return gcPromise->GetFuture();
		                          };

		                          auto shouldRetry = [&](
			                          const PlayerClientConnectorApi::GetConnectionRequestResponse& res) -> bool
		                          {
			                          GetConnectionRequestResponse connection = res.Content;
			                          FString status = GetConnectionRequestResponse::EnumToString(connection.Status);
			                          UE_CLOG(DebugLog, LogBeamClient, Log,
			                                  TEXT("GetConnectionRequest: shouldRetry? (Status [%s] == Pending)"),
			                                  *status);
			                          return connection.Status == GetConnectionRequestStatusEnum::Pending;
		                          };

		                          auto cancellationToken = MakeShared<FBeamCancellationToken>();
		                          if (OutCancellationToken)
		                          {
			                          *OutCancellationToken = cancellationToken;
		                          }

		                          // This call initiates the polling, and waits for the result.
		                          auto pollingResult = PollForResult<
			                          PlayerClientConnectorApi::GetConnectionRequestResponse>(
			                          actionToPerform, shouldRetry, secondsTimeout, 1, cancellationToken).Get();
		                          if (!pollingResult.IsSet())
		                          {
			                          return BeamConnectionResult(EBeamResultType::Error,
			                                                      "Polling for get connection encountered an error or timed out");
		                          }

		                          auto connResult = pollingResult.GetValue();
		                          GetConnectionRequestResponse connection = connResult.Content;

		                          UE_CLOG(DebugLog, LogBeamClient, Log,
		                                  TEXT("Got polling connection request result: %s"),
		                                  *connection.EnumToString(connection.Status));

		                          return BeamConnectionResult(connection.Status);
	                          })
		.Next([Promise, calledFromGameThread](const BeamConnectionResult& result)
		{
			if (calledFromGameThread)
			{
				// Resolve the promise on the game thread for convienence.
				AsyncTask(ENamedThreads::GameThread, [Promise, result]()
				{
					Promise->SetValue(result);
				});
			}
			else
			{
				Promise->SetValue(result);
			}
		});
	return Promise->GetFuture();
}


TFuture<BeamSessionResult> UBeamClient::GetActiveSessionAsync(FString entityId, int chainId,
                                                              TSharedPtr<FBeamCancellationToken>* OutCancellationToken)
{
	auto resultFuture = Async(EAsyncExecution::Thread, [&, entityId, chainId, OutCancellationToken]()
	{
		UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Retrieving active session"));
		auto sessionKeys = GetActiveSessionAndKeysAsync(entityId, chainId).Get();
		if (!sessionKeys.BeamSession.IsSet())
		{
			UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("No active session found"));
			return BeamSessionResult(EBeamResultType::Error, "No active session found");
		}
		else
		{
			return BeamSessionResult(sessionKeys.BeamSession.GetValue());
		}
	});
	return resultFuture;
}


TFuture<BeamOperationResult> UBeamClient::RevokeSessionAsync(FString entityId, FString sessionAddress, int chainId,
                                                             int secondsTimeout,
                                                             TOptional<
	                                                             PlayerClientRevokeSessionRequestInput::AuthProviderEnum>
                                                             authProvider,
                                                             TSharedPtr<FBeamCancellationToken>* OutCancellationToken)
{
	// Track whether this was called from the game thread.
	bool calledFromGameThread = IsInGameThread();

	const auto Promise = MakeShared<TPromise<BeamOperationResult>, ESPMode::ThreadSafe>();

	// Run on another thread so we can use concepts like sleep() when retrying requests without blocking the game thread
	auto resultFuture = Async(EAsyncExecution::Thread,
	                          [&, Promise, entityId, sessionAddress, chainId, authProvider, secondsTimeout,
		                          OutCancellationToken]()
	                          {
		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Retrieving active session"));

		                          PlayerClientSessionsApi::RevokeSessionRequest request;
		                          request.EntityId = entityId;
		                          request.PlayerClientRevokeSessionRequestInput.Address = sessionAddress;
		                          request.PlayerClientRevokeSessionRequestInput.ChainId = chainId;
		                          if (authProvider.IsSet())
		                          {
			                          request.PlayerClientRevokeSessionRequestInput.AuthProvider = authProvider;
		                          }

		                          const auto resPromise = MakeShared<
			                          TPromise<PlayerClientSessionsApi::RevokeSessionResponse>, ESPMode::ThreadSafe>();
		                          auto resFuture = resPromise->GetFuture();
		                          auto httpReq = SessionsApi->RevokeSession(request,
		                                                                    PlayerClientSessionsApi::FRevokeSessionDelegate::CreateLambda(
			                                                                    [&, Promise, secondsTimeout,
				                                                                    OutCancellationToken](
			                                                                    const
			                                                                    PlayerClientSessionsApi::RevokeSessionResponse
			                                                                    & res)
			                                                                    {
				                                                                    resPromise->SetValue(res);
			                                                                    }));

		                          auto res = resFuture.Get();

		                          BeamOperationResult result;
		                          PlayerOperationResponse playerOpRes = res.Content;
		                          if (res.IsSuccessful() && IsOk(res.GetHttpResponseCode()))
		                          {
			                          result = SignOperationUsingBrowserAsync(
				                          playerOpRes, secondsTimeout, OutCancellationToken).Get();
		                          }
		                          else
		                          {
			                          const auto httpResponse = res.GetHttpResponse();
			                          int32 errorCode = res.GetHttpResponseCode();
			                          FString errorBody = TEXT("No response body or invalid response.");
			                          if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
			                          {
				                          errorBody = httpResponse->GetContentAsString();
			                          }

			                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Failed RevokeSessionAsync: %d: %s"),
			                                  errorCode, *errorBody);

			                          result = BeamOperationResult(EBeamResultType::Error, errorBody);
		                          }

		                          // Clear the session key information out of local storage for any result other than rejected.
		                          if (playerOpRes.Status != PlayerOperationStatusEnum::Rejected)
		                          {
			                          ClearLocalSession(entityId);
		                          }

		                          return result;
	                          })
		.Next([Promise, calledFromGameThread](const BeamOperationResult& result)
		{
			if (calledFromGameThread)
			{
				// Resolve the promise on the game thread for convenience.
				AsyncTask(ENamedThreads::GameThread, [Promise, result]()
				{
					Promise->SetValue(result);
				});
			}
			else
			{
				Promise->SetValue(result);
			}
		});
	return Promise->GetFuture();
}


TFuture<BeamSessionResult> UBeamClient::CreateSessionAsync(FString entityId, int chainId, int secondsTimeout,
                                                           TOptional<FDateTime> suggestedExpiry,
                                                           TOptional<
	                                                           PlayerClientGenerateSessionUrlRequestInput::AuthProviderEnum>
                                                           authProvider,
                                                           TOptional<TArray<FString>> contracts,
                                                           TSharedPtr<FBeamCancellationToken>* OutCancellationToken)
{
	// Track whether this was called from the game thread.
	bool calledFromGameThread = IsInGameThread();

	const auto Promise = MakeShared<TPromise<BeamSessionResult>, ESPMode::ThreadSafe>();

	// Run on another thread so we can use concepts like sleep() when retrying requests without blocking the game thread
	auto resultFuture = Async(EAsyncExecution::Thread,
	                          [&, Promise, entityId, chainId, authProvider, secondsTimeout, suggestedExpiry, contracts,
		                          OutCancellationToken]()
	                          {
		                          UE_CLOG(DebugLog, LogBeamClient, Log,
		                                  TEXT("CreateSessionAsync: Retrieving active session: "
			                                  "entityId=%s, chainId=%d, secondsTimeout=%d"), *entityId, chainId,
		                                  secondsTimeout);
		                          {
			                          auto sessionKeys = GetActiveSessionAndKeysAsync(entityId, chainId).Get();
			                          auto& activeSession = sessionKeys.BeamSession;
			                          if (activeSession.IsSet())
			                          {
				                          UE_CLOG(DebugLog, LogBeamClient, Log,
				                                  TEXT("Already has an active session, ending early"));

				                          BeamSessionResult result(EBeamResultType::Success,
				                                                   "Already has an active session");
				                          result.Result = *activeSession;
				                          return result;
			                          }
		                          }

		                          UE_CLOG(DebugLog, LogBeamClient, Log,
		                                  TEXT("No active session found, refreshing local KeyPair"));

		                          // Refresh keypair to make sure we have no conflicts with existing sessions.
		                          KeyPair newKeyPair;
		                          GetOrCreateSigningKeyPair(newKeyPair, entityId, true);

		                          PlayerClientSessionsApi::CreateSessionRequestRequest request;
		                          request.EntityId = entityId;
		                          request.PlayerClientGenerateSessionUrlRequestInput.Address = newKeyPair.GetAddress().
			                          c_str();
		                          request.PlayerClientGenerateSessionUrlRequestInput.ChainId = chainId;

		                          if (authProvider.IsSet())
		                          {
			                          request.PlayerClientGenerateSessionUrlRequestInput.AuthProvider = authProvider;
		                          }

		                          if (suggestedExpiry.IsSet())
		                          {
			                          request.PlayerClientGenerateSessionUrlRequestInput.SuggestedExpiry =
				                          suggestedExpiry.GetValue();
		                          }

		                          if (contracts.IsSet())
		                          {
			                          request.PlayerClientGenerateSessionUrlRequestInput.Contracts = contracts.
				                          GetValue();
		                          }

		                          const auto resPromise = MakeShared<
			                          TPromise<PlayerClientSessionsApi::CreateSessionRequestResponse>,
			                          ESPMode::ThreadSafe>();
		                          auto resFuture = resPromise->GetFuture();
		                          auto httpReq = SessionsApi->CreateSessionRequest(request,
			                          PlayerClientSessionsApi::FCreateSessionRequestDelegate::CreateLambda(
				                          [&, resPromise](
				                          const PlayerClientSessionsApi::CreateSessionRequestResponse& response)
				                          {
					                          resPromise->SetValue(response);
				                          }));

		                          auto res = resFuture.Get();
		                          if (!res.IsSuccessful() || !IsOk(res.GetHttpResponseCode()))
		                          {
			                          const auto httpResponse = res.GetHttpResponse();
			                          int32 errorCode = res.GetHttpResponseCode();
			                          FString errorBody = TEXT("No response body or invalid response.");
			                          if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
			                          {
				                          errorBody = httpResponse->GetContentAsString();
			                          }
			                          UE_CLOG(DebugLog, LogBeamClient, Error,
			                                  TEXT("Failed CreateSessionRequest: %d %s"), errorCode, *errorBody);
			                          return BeamSessionResult(EBeamResultType::Error,
			                                                   "Failed to create session request");
		                          }

		                          GenerateSessionRequestResponse session = res.Content;
		                          if (session.Status == GenerateSessionRequestStatusEnum::Error)
		                          {
			                          const auto httpResponse = res.GetHttpResponse();
			                          FString errorBody = TEXT("No response body or invalid response.");
			                          if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
			                          {
				                          errorBody = httpResponse->GetContentAsString();
			                          }

			                          UE_CLOG(DebugLog, LogBeamClient, Error,
			                                  TEXT("Failed creating session request: %s"), *errorBody);
			                          return BeamSessionResult(EBeamResultType::Error,
			                                                   "CreateSession returned status=Error");
		                          }

		                          UE_CLOG(DebugLog, LogBeamClient, Log,
		                                  TEXT("Created session request: %s to check for session result"), *session.Id);
		                          PlayerClientGenerateSessionRequestResponse beamSessionRequest = session;

		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Opening %s"), *beamSessionRequest.Url);

		                          // Open "identity.onbeam.com".
		                          FString launchErrors = LaunchURL(beamSessionRequest.Url);

		                          // Start polling for results of the operation.
		                          UE_CLOG(DebugLog, LogBeamClient, Log,
		                                  TEXT("Started polling for Session creation result"));

		                          bool error = false;

		                          PlayerClientSessionsApi::GetSessionRequestRequest gsRequest;
		                          gsRequest.RequestId = beamSessionRequest.Id;

		                          // This action will be polled in the PollForResult call below.
		                          auto actionToPerform = [&, gsRequest
			                          ]() -> TFuture<PlayerClientSessionsApi::GetSessionRequestResponse>
		                          {
			                          const auto gsPromise = MakeShared<
				                          TPromise<PlayerClientSessionsApi::GetSessionRequestResponse>,
				                          ESPMode::ThreadSafe>();
			                          SessionsApi->GetSessionRequest(gsRequest,
			                                                         PlayerClientSessionsApi::FGetSessionRequestDelegate::CreateLambda(
				                                                         [&, gsPromise](
				                                                         const
				                                                         PlayerClientSessionsApi::GetSessionRequestResponse
				                                                         & res)
				                                                         {
					                                                         FString resBody = res.GetHttpResponse()->
						                                                         GetContentAsString();
					                                                         UE_CLOG(DebugLog, LogBeamClient, Log,
						                                                         TEXT("GetSessionRequest: response=%s"),
						                                                         *resBody);
					                                                         gsPromise->SetValue(res);
				                                                         }));
			                          return gsPromise->GetFuture();
		                          };

		                          // This controls whether the above action should continue to poll.
		                          auto shouldRetry = [&](
			                          const PlayerClientSessionsApi::GetSessionRequestResponse& res) -> bool
		                          {
			                          GetSessionRequestResponse session = res.Content;
			                          FString status = GetSessionRequestResponse::EnumToString(session.Status);
			                          UE_CLOG(DebugLog, LogBeamClient, Log,
			                                  TEXT("GetSessionRequest: shouldRetry? (Status [%s] == Pending)"),
			                                  *status);
			                          return session.Status == GetSessionRequestStatusEnum::Pending;
		                          };

		                          auto cancellationToken = MakeShared<FBeamCancellationToken>();
		                          if (OutCancellationToken)
		                          {
			                          *OutCancellationToken = cancellationToken;
		                          }

		                          // This call initiates the polling, and waits for the result.
		                          auto pollingResult = PollForResult<
			                          PlayerClientSessionsApi::GetSessionRequestResponse>(
			                          actionToPerform, shouldRetry, secondsTimeout, 1, cancellationToken).Get();
		                          if (!pollingResult.IsSet())
		                          {
			                          return BeamSessionResult(EBeamResultType::Error,
			                                                   "Polling for created session encountered an error or timed out");
		                          }

		                          BeamSessionResult beamResultModel;
		                          switch (pollingResult.GetValue().Content.Status)
		                          {
		                          case GetSessionRequestStatusEnum::Accepted:
			                          beamResultModel.Status = EBeamResultType::Success;
			                          break;
		                          case GetSessionRequestStatusEnum::Error:
			                          beamResultModel.Status = EBeamResultType::Error;
			                          beamResultModel.Error = "Encountered an error when requesting a session";
			                          error = true;
			                          break;
		                          case GetSessionRequestStatusEnum::Pending:
		                          default:
			                          beamResultModel.Status = EBeamResultType::Error;
			                          beamResultModel.Error = TEXT(
				                          "Polling for operation encountered an error or timed out");
			                          break;
		                          }

		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Retrieving newly created Session"));

		                          // Retrieve newly created session.
		                          if (!error)
		                          {
			                          auto sessionKeys = GetActiveSessionAndKeysAsync(entityId, chainId).Get();
			                          if (sessionKeys.BeamSession.IsSet())
			                          {
				                          auto& beamSession = sessionKeys.BeamSession.GetValue();
				                          beamResultModel.Result = beamSession;
				                          UE_CLOG(DebugLog, LogBeamClient, Log,
				                                  TEXT("Retrieved a session: %s, valid from: %s, to: %s"),
				                                  *beamSession.SessionAddress,
				                                  *FText::AsDateTime(beamSession.StartTime).ToString(),
				                                  *FText::AsDateTime(beamSession.EndTime).ToString());
			                          }
			                          else
			                          {
				                          beamResultModel.Error = "Could not retrieve session after it was created";
				                          beamResultModel.Status = EBeamResultType::Error;
			                          }
		                          }

		                          return beamResultModel;
	                          })
		.Next([Promise, calledFromGameThread](const BeamSessionResult& result)
		{
			if (calledFromGameThread)
			{
				// Resolve the promise on the game thread for convenience.
				AsyncTask(ENamedThreads::GameThread, [Promise, result]()
				{
					Promise->SetValue(result);
				});
			}
			else
			{
				Promise->SetValue(result);
			}
		});
	return Promise->GetFuture();
}


TFuture<BeamOperationResult> UBeamClient::SignOperationAsync(FString entityId,
                                                             FString operationId,
                                                             int chainId,
                                                             EBeamOperationSigningBy signingBy,
                                                             int secondsTimeout,
                                                             TSharedPtr<FBeamCancellationToken>* OutCancellationToken)
{
	// Track whether this was called from the game thread.
	bool calledFromGameThread = IsInGameThread();

	const auto Promise = MakeShared<TPromise<BeamOperationResult>, ESPMode::ThreadSafe>();
	auto resultFuture = Async(EAsyncExecution::Thread,
	                          [&, Promise, entityId, operationId, chainId, signingBy, secondsTimeout,
		                          OutCancellationToken]()
	                          {
		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Retrieving operation(%s)"), *operationId);

		                          PlayerClientOperationApi::GetOperationRequest getOpRequest;
		                          getOpRequest.OpId = operationId;

		                          const auto resPromise = MakeShared<
			                          TPromise<GetOperationResponse>, ESPMode::ThreadSafe>();
		                          auto resFuture = resPromise->GetFuture();
		                          auto httpReq = OperationApi->GetOperation(getOpRequest,
		                                                                    PlayerClientOperationApi::FGetOperationDelegate::CreateLambda(
			                                                                    [&, resPromise](
			                                                                    const GetOperationResponse& res)
			                                                                    {
				                                                                    FString resBody = res.
					                                                                    GetHttpResponse()->
					                                                                    GetContentAsString();
				                                                                    UE_CLOG(DebugLog, LogBeamClient,
					                                                                    Log,
					                                                                    TEXT("GetOperation: response=%s"
					                                                                    ), *resBody);
				                                                                    resPromise->SetValue(res);
			                                                                    }));

		                          auto res = resFuture.Get();
		                          if (!res.IsSuccessful() || !IsOk(res.GetHttpResponseCode()))
		                          {
			                          if (res.GetHttpResponseCode() == 404)
			                          {
				                          UE_CLOG(DebugLog, LogBeamClient, Log,
				                                  TEXT("No operation(%s) was found, ending"), *operationId);
				                          return BeamOperationResult(EBeamResultType::Error, "Operation was not found");
			                          }

			                          const auto httpResponse = res.GetHttpResponse();
			                          int32 errorCode = res.GetHttpResponseCode();
			                          FString errorBody = TEXT("No response body or invalid response.");
			                          if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
			                          {
				                          errorBody = httpResponse->GetContentAsString();
			                          }

			                          FString errorMessage = FString::Printf(
				                          TEXT("Error retrieving operation(%s): code=%d, response=%s"), *operationId,
				                          errorCode, *errorBody);
			                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("%s"), *errorMessage);
			                          return BeamOperationResult(EBeamResultType::Error, errorMessage);
		                          }

		                          PlayerOperationResponse operation = res.Content;

		                          // if more than one actions and at least one has a signature, otherwise try Browser
		                          bool hasSignaturesToSign = operation.Actions.Num() > 0
			                          && operation.Actions.ContainsByPredicate([](const auto& action)
			                          {
				                          return action.Signature.IsSet() || action.Transaction.IsSet();
			                          });
		                          if (signingBy == EBeamOperationSigningBy::Auto || signingBy ==
			                          EBeamOperationSigningBy::Session && hasSignaturesToSign)
		                          {
			                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Retrieving active session"));
			                          auto sessionKeys = GetActiveSessionAndKeysAsync(entityId, chainId).Get();
			                          if (sessionKeys.BeamSession.IsSet() && sessionKeys.KeyPair.IsSet())
			                          {
				                          UE_CLOG(DebugLog, LogBeamClient, Log,
				                                  TEXT("Has an active session until: %s, using it to sign the operation"
				                                  ), *sessionKeys.BeamSession.GetValue().EndTime.ToString());
				                          return SignOperationUsingSessionAsync(operation, sessionKeys.KeyPair).Get();
			                          }
		                          }

		                          if (signingBy == EBeamOperationSigningBy::Auto || signingBy ==
			                          EBeamOperationSigningBy::Browser)
		                          {
			                          UE_CLOG(DebugLog, LogBeamClient, Log,
			                                  TEXT("No active session found, using browser to sign the operation"));
			                          return SignOperationUsingBrowserAsync(
				                          operation, secondsTimeout, OutCancellationToken).Get();
		                          }

		                          BeamOperationResult result;
		                          result.Result = PlayerOperationStatusEnum::Error;
		                          result.Status = EBeamResultType::Error;
		                          result.Error = FString::Printf(
			                          TEXT("No active session found, signingBy set to %s"),
			                          *UEnum::GetDisplayValueAsText(signingBy).ToString());
		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("%s"), *result.Error);
		                          return result;
	                          })
		.Next([Promise, calledFromGameThread](const BeamOperationResult& result)
		{
			if (calledFromGameThread)
			{
				// Resolve the promise on the game thread for convienence.
				AsyncTask(ENamedThreads::GameThread, [Promise, result]()
				{
					Promise->SetValue(result);
				});
			}
			else
			{
				Promise->SetValue(result);
			}
		});
	return Promise->GetFuture();
}


void UBeamClient::ClearLocalSession(FString EntityId)
{
	if (Storage && !EntityId.IsEmpty())
	{
		const FString entitySigningKey = FBeamConstants::Storage::BeamSigningKey + EntityId;
		Storage->Delete(entitySigningKey);
		Storage->Save();
	}
}


TFuture<BeamOperationResult> UBeamClient::SignOperationUsingBrowserAsync(
	PlayerOperationResponse operation,
	int secondsTimeout,
	TSharedPtr<FBeamCancellationToken>* OutCancellationToken)
{
	// Track whether this was called from the game thread.
	bool calledFromGameThread = IsInGameThread();

	const auto Promise = MakeShared<TPromise<BeamOperationResult>, ESPMode::ThreadSafe>();

	// Run on another thread so we can use concepts like sleep() when retrying requests without blocking the game thread
	auto resultFuture = Async(EAsyncExecution::Thread, [&, Promise, operation, secondsTimeout, OutCancellationToken]()
		{
			UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Opening %s..."), *operation.Url);

			// Open identity.onbeam.com, give it operation id
			LaunchURL(operation.Url);

			// Start polling for results of the operation.
			UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Start polling for results of the operation"));

			bool error = false;

			PlayerClientOperationApi::GetOperationRequest getOpRequest;
			getOpRequest.OpId = operation.Id;

			// This action will be polled in the PollForResult call below.
			auto actionToPerform = [&, getOpRequest]() -> TFuture<GetOperationResponse>
			{
				const auto getOpPromise = MakeShared<TPromise<GetOperationResponse>, ESPMode::ThreadSafe>();
				OperationApi->GetOperation(getOpRequest,
				                           PlayerClientOperationApi::FGetOperationDelegate::CreateLambda(
					                           [&, getOpPromise](const GetOperationResponse& res)
					                           {
						                           FString resBody = res.GetHttpResponse()->GetContentAsString();
						                           UE_CLOG(DebugLog, LogBeamClient, Log,
						                                   TEXT("GetOperation: response=%s"), *resBody);
						                           getOpPromise->SetValue(res);
					                           }));
				return getOpPromise->GetFuture();
			};


			// This controls whether the above action should continue to poll.
			FDateTime now = FDateTime::UtcNow();
			auto shouldRetry = [&, now](const GetOperationResponse& res) -> bool
			{
				const auto httpResponse = res.GetHttpResponse();
				PlayerOperationResponse commonOpRes = res.Content;
				FString status = PlayerOperationResponse::EnumToString(commonOpRes.Status);
				FString resBody = TEXT("No response body or invalid response.");
				if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
				{
					resBody = httpResponse->GetContentAsString();
				}
				UE_CLOG(DebugLog, LogBeamClient, Log,
				        TEXT("GetOperation: shouldRetry? (Status [%s] == Pending OR != (Executed OR Rejected))\n%s"),
				        *status, *resBody);

				// Always 
				if (commonOpRes.Status == PlayerOperationStatusEnum::Pending)
				{
					return true;
				}

				// stop retrying in all other cases
				return false;
			};

			auto cancellationToken = MakeShared<FBeamCancellationToken>();
			if (OutCancellationToken)
			{
				*OutCancellationToken = cancellationToken;
			}

			// This call initiates the polling, and waits for the result.
			auto pollingResult = PollForResult<GetOperationResponse>(actionToPerform, shouldRetry, secondsTimeout, 1,
			                                                         cancellationToken).Get();
			if (!pollingResult.IsSet())
			{
				return BeamOperationResult(EBeamResultType::Error,
				                           "Polling for sign operation encountered an error or timed out");
			}

			auto pollingStatus = pollingResult
				                     ? pollingResult.GetValue().Content.Status
				                     : PlayerOperationStatusEnum::Error;
			FString status = PlayerOperationResponse::EnumToString(pollingStatus);

			UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Got operation(%s) result: %s"), *operation.Id, *status);
			auto beamResult = BeamOperationResult(pollingStatus);

			switch (pollingStatus)
			{
			case PlayerOperationStatusEnum::Executed:
			case PlayerOperationStatusEnum::Rejected:
			case PlayerOperationStatusEnum::_Signed:
				beamResult.Status = EBeamResultType::Success;
				break;
			case PlayerOperationStatusEnum::Error:
				beamResult.Status = EBeamResultType::Error;
				beamResult.Error = TEXT("Operation encountered an error");
				break;
			case PlayerOperationStatusEnum::Pending:
			default:
				beamResult.Status = EBeamResultType::Error;
				beamResult.Error = TEXT("Polling for operation encountered an error or timed out");
				break;
			}

			return beamResult;
		})
		.Next([Promise, calledFromGameThread](const BeamOperationResult& result)
		{
			if (calledFromGameThread)
			{
				// Resolve the promise on the game thread for convienence.
				AsyncTask(ENamedThreads::GameThread, [Promise, result]()
				{
					Promise->SetValue(result);
				});
			}
			else
			{
				Promise->SetValue(result);
			}
		});

	return Promise->GetFuture();
}


TFuture<BeamOperationResult> UBeamClient::SignOperationUsingSessionAsync(
	PlayerOperationResponse operation, KeyPair activeSessionKeyPair,
	TSharedPtr<FBeamCancellationToken>* OutCancellationToken)
{
	// Track whether this was called from the game thread.
	bool calledFromGameThread = IsInGameThread();

	const auto Promise = MakeShared<TPromise<BeamOperationResult>, ESPMode::ThreadSafe>();

	// Run on another thread so we can use concepts like sleep() when retrying requests without blocking the game thread
	auto resultFuture = Async(EAsyncExecution::Thread, [&, Promise, operation, OutCancellationToken]()
	                          {
		                          if (operation.Actions.IsEmpty())
		                          {
			                          BeamOperationResult result;
			                          result.Result = PlayerOperationStatusEnum::Error;
			                          result.Status = EBeamResultType::Error;
			                          result.Error = TEXT("Operation has no actions to sign");
			                          UE_CLOG(DebugLog, LogBeamClient, Log,
			                                  TEXT("Operation(%s) has no actions to sign, ending"),
			                                  *operation.Id);
			                          return result;
		                          }

		                          ConfirmOperationRequest confirmationModel;
		                          confirmationModel.Status = ConfirmOperationStatusEnum::Pending;
		                          confirmationModel.Actions = TArray<
			                          ConfirmOperationRequestAction>();

		                          for (auto& action : operation.Actions)
		                          {
			                          UE_CLOG(DebugLog, LogBeamClient, Log,
			                                  TEXT("Signing operation(%s) actions(%s)..."),
			                                  *operation.Id,
			                                  *action.Id);

			                          FString signature;
			                          if (action.Type == PlayerOperationResponseAction::TypeEnum::SessionRevoke)
			                          {
				                          FString errorLogMessage = TEXT(
					                          "Revoke Session Operation has to be performed via RevokeSessionAsync() method only");
				                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("%s"), *errorLogMessage);
				                          FString errorMessage = FString::Printf(
					                          TEXT("Encountered an exception while signing %s"),
					                          *PlayerOperationResponseAction::EnumToString(action.Type));
				                          return BeamOperationResult(EBeamResultType::Error, errorMessage);
			                          }

			                          if (!action.Signature.IsSet())
			                          {
				                          FString errorLogMessage = TEXT(
					                          "Operation action has no Signature object, was probably not meant to be signed");
				                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("%s"), *errorLogMessage);
				                          FString errorMessage = FString::Printf(
					                          TEXT("Encountered an exception while signing %s"),
					                          *PlayerOperationResponseAction::EnumToString(action.Type));
				                          return BeamOperationResult(EBeamResultType::Error, errorMessage);
			                          }

			                          PlayerClientPlayerOperationActionSignature signatureObj = action.Signature.
				                          GetValue();
			                          switch (signatureObj.Type)
			                          {
			                          case PlayerClientPlayerOperationActionSignature::TypeEnum::Message:
				                          {
					                          auto message = std::string(
						                          TCHAR_TO_UTF8(*signatureObj.Data.GetValue().Get()->AsString()));
					                          signature = activeSessionKeyPair.Sign(message).c_str();
					                          break;
				                          }
			                          case PlayerClientPlayerOperationActionSignature::TypeEnum::TypedData:
				                          {
					                          auto hash = std::string(TCHAR_TO_UTF8(*signatureObj.Hash.GetValue()));
					                          signature = activeSessionKeyPair.SignMarketplaceTransactionHash(hash).
					                                                           c_str();
					                          break;
				                          }
			                          default:
				                          {
					                          UE_CLOG(DebugLog, LogBeamClient, Log,
					                                  TEXT(
						                                  "Encountered an error when signing action(%s): Unhandled Type"
					                                  ),
					                                  *action.Id);
					                          FString errorMessage = FString::Printf(
						                          TEXT("Encountered an exception while approving %s"),
						                          *PlayerOperationResponseAction::EnumToString(action.Type));
					                          return BeamOperationResult(EBeamResultType::Error, errorMessage);
				                          }
			                          }

			                          ConfirmOperationRequestAction inner;
			                          inner.Id = action.Id;
			                          inner.Signature = signature;

			                          confirmationModel.Actions.GetValue().Add(inner);
		                          }

		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Confirming operation(%s)..."),
		                                  *operation.Id);

		                          PlayerClientOperationApi::ProcessOperationRequest request;
		                          request.OpId = operation.Id;
		                          request.PlayerClientConfirmOperationRequest = confirmationModel;

		                          const auto resPromise = MakeShared<
			                          TPromise<PlayerClientOperationApi::ProcessOperationResponse>,
			                          ESPMode::ThreadSafe>();
		                          auto resFuture = resPromise->GetFuture();
		                          auto httpReq = OperationApi->ProcessOperation(request,
			                          PlayerClientOperationApi::FProcessOperationDelegate::CreateLambda(
				                          [&, resPromise, request](
				                          const PlayerClientOperationApi::ProcessOperationResponse&
				                          res)
				                          {
					                          FString resBody = res.GetHttpResponse()->
					                                                GetContentAsString();
					                          UE_CLOG(DebugLog, LogBeamClient, Log,
					                                  TEXT("ProcessOperation(%s): response=%s"),
					                                  *request.OpId, *resBody);
					                          resPromise->SetValue(res);
				                          }));

		                          auto res = resFuture.Get();
		                          if (!res.IsSuccessful() || !IsOk(res.GetHttpResponseCode()))
		                          {
			                          FString errorMessage = FString::Printf(
				                          TEXT("Encountered unknown error when confirming operation %s"),
				                          *operation.Id);
			                          const auto httpResponse = res.GetHttpResponse();
			                          FString resBody = TEXT("No response body or invalid response.");
			                          if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
			                          {
				                          resBody = httpResponse->GetContentAsString();
			                          }
			                          UE_CLOG(DebugLog, LogBeamClient, Log,
			                                  TEXT("Confirming operation(%s) encountered an error: %s"),
			                                  *operation.Id, *resBody);
			                          return BeamOperationResult(EBeamResultType::Error, errorMessage);
		                          }

		                          PlayerOperationResponse commonOpRes = res.Content;
		                          auto didFail = commonOpRes.Status != PlayerOperationStatusEnum::Executed &&
			                          commonOpRes.Status != PlayerOperationStatusEnum::_Signed &&
			                          commonOpRes.Status != PlayerOperationStatusEnum::Pending;

		                          FString status = PlayerOperationResponse::EnumToString(commonOpRes.Status);
		                          UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("Confirmed operation(%s), status: %s"),
		                                  *commonOpRes.Id,
		                                  *status);
		                          BeamOperationResult beamResult;
		                          beamResult.Status = didFail ? EBeamResultType::Error : EBeamResultType::Success;
		                          beamResult.Result = commonOpRes.Status;

		                          return beamResult;
	                          }

		)
		.
		Next([Promise, calledFromGameThread]
		(
			const BeamOperationResult& result
		)
			{
				if (calledFromGameThread)
				{
					// Resolve the promise on the game thread for convienence.
					AsyncTask(ENamedThreads::GameThread, [Promise, result]()
					{
						Promise->SetValue(result);
					});
				}
				else
				{
					Promise->SetValue(result);
				}
			});

	return
		Promise
		->
		GetFuture();
}


TFuture<FBeamSessionAndKeyPair> UBeamClient::GetActiveSessionAndKeysAsync(FString entityId, int chainId)
{
	auto resultFuture = Async(EAsyncExecution::Thread, [&, entityId, chainId]()
	{
		FBeamSessionAndKeyPair sessionKeys;
		KeyPair& keyPair = sessionKeys.KeyPair;

		GetOrCreateSigningKeyPair(keyPair, entityId);

		// user might've revoked the session outside the game so
		// get active session for current keyPair from the API
		PlayerClientSessionsApi::GetActiveSessionV2Request request;
		request.EntityId = entityId;
		request.AccountAddress = keyPair.GetAddress().c_str();
		request.ChainId = chainId;

		const auto resPromise = MakeShared<TPromise<PlayerClientSessionsApi::GetActiveSessionV2Response>,
		                                   ESPMode::ThreadSafe>();
		auto resFuture = resPromise->GetFuture();
		auto httpReq = SessionsApi->GetActiveSessionV2(request,
		                                               PlayerClientSessionsApi::FGetActiveSessionV2Delegate::CreateLambda(
			                                               [&, resPromise](
			                                               const PlayerClientSessionsApi::GetActiveSessionV2Response&
			                                               response)
			                                               {
				                                               resPromise->SetValue(response);
			                                               }));

		auto res = resFuture.Get();
		if (res.IsSuccessful() && IsOk(res.GetHttpResponseCode()))
		{
			if (res.Content.Session.IsSet())
			{
				sessionKeys.BeamSession = FBeamSession(res);
			}
			else
			{
				UE_CLOG(DebugLog, LogBeamClient, Error, TEXT("GetActiveSessionInfo did not return an active session"));
			}
		}
		else
		{
			const auto httpResponse = res.GetHttpResponse();
			int32 errorCode = res.GetHttpResponseCode();
			FString errorBody = TEXT("No response body or invalid response.");
			if (httpResponse.IsValid() && httpResponse->GetContentLength() > 0)
			{
				errorBody = httpResponse->GetContentAsString();
			}

			UE_CLOG(DebugLog, LogBeamClient, Error, TEXT("GetActiveSessionInfo returned: %d %s"), errorCode,
			        *errorBody);
		}

		if (sessionKeys.BeamSession.IsSet())
		{
			FBeamSession& beamSession = sessionKeys.BeamSession.GetValue();

			// Make sure session we just retrieved is valid and owned by current KeyPair.
			if (beamSession.IsValidNow() && beamSession.IsOwnedBy(keyPair))
			{
				return sessionKeys;
			}
		}

		// If session is not valid or owned by different KeyPair, remove it from cache.
		ClearLocalSession(entityId);
		sessionKeys.BeamSession.Reset();
		return sessionKeys;
	});
	return resultFuture;
}


void UBeamClient::GetOrCreateSigningKeyPair(KeyPair& OutKeyPair, FString InEntityId, bool InRefresh)
{
	FString entitySigningKey = FBeamConstants::Storage::BeamSigningKey + InEntityId;
	if (!InRefresh)
	{
		FString privateKey = Storage->Get(entitySigningKey);
		if (!privateKey.IsEmpty())
		{
			OutKeyPair.Initialize(std::string(TCHAR_TO_UTF8(*privateKey)));
			return;
		}
	}

	OutKeyPair.Generate();

	// Allows debugging the generated keys for debug and development builds. (1 = On, 0 = Off [default])
#define DEBUG_KEY_GEN 0
#if !UE_BUILD_SHIPPING && DEBUG_KEY_GEN
	{
		FString address = OutKeyPair.GetAddress().c_str();
		FString publicKey = OutKeyPair.GetPublicKeyHex().c_str();
		FString privateKey = OutKeyPair.GetPrivateKeyHex().c_str();
		UE_CLOG(DebugLog, LogBeamClient, Log, TEXT("KeyPair:\n  address:%s\n  public: %s\n  private: %s"), *address, *publicKey, *privateKey);
	}
#endif
#undef DEBUG_KEY_GEN

	Storage->Set(entitySigningKey, OutKeyPair.GetPrivateKeyHex().c_str());
	Storage->Save();
}

UE_ENABLE_OPTIMIZATION
