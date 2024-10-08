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

#pragma once

#include "CoreMinimal.h"
#include "PlayerClientBaseModel.h"

namespace OpenAPI
{

class PLAYERCLIENT_API PlayerClientTransactionsApi
{
public:
	PlayerClientTransactionsApi();
	~PlayerClientTransactionsApi();

	/* Sets the URL Endpoint.
	* Note: several fallback endpoints can be configured in request retry policies, see Request::SetShouldRetry */
	void SetURL(const FString& Url);

	/* Adds global header params to all requests */
	void AddHeaderParam(const FString& Key, const FString& Value);
	void ClearHeaderParams();

	/* Sets the retry manager to the user-defined retry manager. User must manage the lifetime of the retry manager.
	* If no retry manager is specified and a request needs retries, a default retry manager will be used.
	* See also: Request::SetShouldRetry */
	void SetHttpRetryManager(FHttpRetrySystem::FManager& RetryManager);
	FHttpRetrySystem::FManager& GetHttpRetryManager();

	class CreateUserTransactionRequest;
	class CreateUserTransactionResponse;
	class GetTransactionRequest;
	class GetTransactionResponse;
	class GetTransactionsRequest;
	class GetTransactionsResponse;
	class GetUserTransactionsRequest;
	class GetUserTransactionsResponse;
	
    DECLARE_DELEGATE_OneParam(FCreateUserTransactionDelegate, const CreateUserTransactionResponse&);
    DECLARE_DELEGATE_OneParam(FGetTransactionDelegate, const GetTransactionResponse&);
    DECLARE_DELEGATE_OneParam(FGetTransactionsDelegate, const GetTransactionsResponse&);
    DECLARE_DELEGATE_OneParam(FGetUserTransactionsDelegate, const GetUserTransactionsResponse&);
    
    FHttpRequestPtr CreateUserTransaction(const CreateUserTransactionRequest& Request, const FCreateUserTransactionDelegate& Delegate = FCreateUserTransactionDelegate()) const;
    FHttpRequestPtr GetTransaction(const GetTransactionRequest& Request, const FGetTransactionDelegate& Delegate = FGetTransactionDelegate()) const;
    FHttpRequestPtr GetTransactions(const GetTransactionsRequest& Request, const FGetTransactionsDelegate& Delegate = FGetTransactionsDelegate()) const;
    FHttpRequestPtr GetUserTransactions(const GetUserTransactionsRequest& Request, const FGetUserTransactionsDelegate& Delegate = FGetUserTransactionsDelegate()) const;
    
private:
    void OnCreateUserTransactionResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FCreateUserTransactionDelegate Delegate) const;
    void OnGetTransactionResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetTransactionDelegate Delegate) const;
    void OnGetTransactionsResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetTransactionsDelegate Delegate) const;
    void OnGetUserTransactionsResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetUserTransactionsDelegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}
