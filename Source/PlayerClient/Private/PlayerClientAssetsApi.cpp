/**
 * Player API
 * The Player API is a service to integrate your game with Beam
 *
 * OpenAPI spec version: 1.0.0
 * 
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#include "PlayerClientAssetsApi.h"

#include "PlayerClientAssetsApiOperations.h"
#include "PlayerClientModule.h"

#include "HttpModule.h"
#include "Serialization/JsonSerializer.h"

namespace OpenAPI
{

PlayerClientAssetsApi::PlayerClientAssetsApi()
: Url(TEXT("http://localhost"))
{
}

PlayerClientAssetsApi::~PlayerClientAssetsApi() {}

void PlayerClientAssetsApi::SetURL(const FString& InUrl)
{
	Url = InUrl;
}

void PlayerClientAssetsApi::AddHeaderParam(const FString& Key, const FString& Value)
{
	AdditionalHeaderParams.Add(Key, Value);
}

void PlayerClientAssetsApi::ClearHeaderParams()
{
	AdditionalHeaderParams.Reset();
}

bool PlayerClientAssetsApi::IsValid() const
{
	if (Url.IsEmpty())
	{
		UE_LOG(LogPlayerClient, Error, TEXT("PlayerClientAssetsApi: Endpoint Url is not set, request cannot be performed"));
		return false;
	}

	return true;
}

void PlayerClientAssetsApi::SetHttpRetryManager(FHttpRetrySystem::FManager& InRetryManager)
{
	if (RetryManager != &InRetryManager)
	{
		DefaultRetryManager.Reset();
		RetryManager = &InRetryManager;
	}
}

FHttpRetrySystem::FManager& PlayerClientAssetsApi::GetHttpRetryManager()
{
	checkf(RetryManager, TEXT("PlayerClientAssetsApi: RetryManager is null.  You may have meant to set it with SetHttpRetryManager first, or you may not be using a custom RetryManager at all."))
	return *RetryManager;
}

FHttpRequestRef PlayerClientAssetsApi::CreateHttpRequest(const Request& Request) const
{
	if (!Request.GetRetryParams().IsSet())
	{
		return FHttpModule::Get().CreateRequest();
	}
	else
	{
		if (!RetryManager)
		{
			// Create default retry manager if none was specified
			DefaultRetryManager = MakeUnique<HttpRetryManager>(6, 60);
			RetryManager = DefaultRetryManager.Get();
		}

		const HttpRetryParams& Params = Request.GetRetryParams().GetValue();
		return RetryManager->CreateRequest(Params.RetryLimitCountOverride, Params.RetryTimeoutRelativeSecondsOverride, Params.RetryResponseCodes, Params.RetryVerbs, Params.RetryDomains);
	}
}

void PlayerClientAssetsApi::HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const
{
	InOutResponse.SetHttpResponse(HttpResponse);
	InOutResponse.SetSuccessful(bSucceeded);

	if (bSucceeded && HttpResponse.IsValid())
	{
		InOutResponse.SetHttpResponseCode((EHttpResponseCodes::Type)HttpResponse->GetResponseCode());
		FString ContentType = HttpResponse->GetContentType();
		FString Content;

		if (ContentType.IsEmpty())
		{
			return; // Nothing to parse
		}
		else if (ContentType.StartsWith(TEXT("application/json")) || ContentType.StartsWith("text/json"))
		{
			Content = HttpResponse->GetContentAsString();

			TSharedPtr<FJsonValue> JsonValue;
			auto Reader = TJsonReaderFactory<>::Create(Content);

			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
			{
				if (InOutResponse.FromJson(JsonValue))
					return; // Successfully parsed
			}
		}
		else if(ContentType.StartsWith(TEXT("text/plain")))
		{
			Content = HttpResponse->GetContentAsString();
			InOutResponse.SetResponseString(Content);
			return; // Successfully parsed
		}

		// Report the parse error but do not mark the request as unsuccessful. Data could be partial or malformed, but the request succeeded.
		UE_LOG(LogPlayerClient, Error, TEXT("Failed to deserialize Http response content (type:%s):\n%s"), *ContentType , *Content);
		return;
	}

	// By default, assume we failed to establish connection
	InOutResponse.SetHttpResponseCode(EHttpResponseCodes::RequestTimeout);
}

FHttpRequestPtr PlayerClientAssetsApi::GetAsset(const GetAssetRequest& Request, const FGetAssetDelegate& Delegate /*= FGetAssetDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetAssetResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetAssetResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetAssetDelegate Delegate) const
{
	GetAssetResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::GetAssetsForContract(const GetAssetsForContractRequest& Request, const FGetAssetsForContractDelegate& Delegate /*= FGetAssetsForContractDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetAssetsForContractResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetAssetsForContractResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetAssetsForContractDelegate Delegate) const
{
	GetAssetsForContractResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::GetAttributes(const GetAttributesRequest& Request, const FGetAttributesDelegate& Delegate /*= FGetAttributesDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetAttributesResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetAttributesResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetAttributesDelegate Delegate) const
{
	GetAttributesResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::GetOwnerAssets(const GetOwnerAssetsRequest& Request, const FGetOwnerAssetsDelegate& Delegate /*= FGetOwnerAssetsDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetOwnerAssetsResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetOwnerAssetsResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetOwnerAssetsDelegate Delegate) const
{
	GetOwnerAssetsResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::GetOwners(const GetOwnersRequest& Request, const FGetOwnersDelegate& Delegate /*= FGetOwnersDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetOwnersResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetOwnersResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetOwnersDelegate Delegate) const
{
	GetOwnersResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::GetUserAssetsForGamePost(const GetUserAssetsForGamePostRequest& Request, const FGetUserAssetsForGamePostDelegate& Delegate /*= FGetUserAssetsForGamePostDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetUserAssetsForGamePostResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetUserAssetsForGamePostResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetUserAssetsForGamePostDelegate Delegate) const
{
	GetUserAssetsForGamePostResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::GetUserCurrencies(const GetUserCurrenciesRequest& Request, const FGetUserCurrenciesDelegate& Delegate /*= FGetUserCurrenciesDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetUserCurrenciesResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetUserCurrenciesResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetUserCurrenciesDelegate Delegate) const
{
	GetUserCurrenciesResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::GetUserNativeCurrency(const GetUserNativeCurrencyRequest& Request, const FGetUserNativeCurrencyDelegate& Delegate /*= FGetUserNativeCurrencyDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnGetUserNativeCurrencyResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnGetUserNativeCurrencyResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetUserNativeCurrencyDelegate Delegate) const
{
	GetUserNativeCurrencyResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::TransferAsset(const TransferAssetRequest& Request, const FTransferAssetDelegate& Delegate /*= FTransferAssetDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnTransferAssetResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnTransferAssetResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FTransferAssetDelegate Delegate) const
{
	TransferAssetResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::TransferNativeToken(const TransferNativeTokenRequest& Request, const FTransferNativeTokenDelegate& Delegate /*= FTransferNativeTokenDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnTransferNativeTokenResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnTransferNativeTokenResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FTransferNativeTokenDelegate Delegate) const
{
	TransferNativeTokenResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr PlayerClientAssetsApi::TransferToken(const TransferTokenRequest& Request, const FTransferTokenDelegate& Delegate /*= FTransferTokenDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &PlayerClientAssetsApi::OnTransferTokenResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void PlayerClientAssetsApi::OnTransferTokenResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FTransferTokenDelegate Delegate) const
{
	TransferTokenResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

}
