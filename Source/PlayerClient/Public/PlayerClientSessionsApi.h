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

class PLAYERCLIENT_API PlayerClientSessionsApi
{
public:
	PlayerClientSessionsApi();
	~PlayerClientSessionsApi();

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

	class CreateSessionRequestRequest;
	class CreateSessionRequestResponse;
	class GetActiveSessionRequest;
	class GetActiveSessionResponse;
	class GetAllActiveSessionsRequest;
	class GetAllActiveSessionsResponse;
	class GetSessionRequestRequest;
	class GetSessionRequestResponse;
	class RevokeSessionRequest;
	class RevokeSessionResponse;
	
    DECLARE_DELEGATE_OneParam(FCreateSessionRequestDelegate, const CreateSessionRequestResponse&);
    DECLARE_DELEGATE_OneParam(FGetActiveSessionDelegate, const GetActiveSessionResponse&);
    DECLARE_DELEGATE_OneParam(FGetAllActiveSessionsDelegate, const GetAllActiveSessionsResponse&);
    DECLARE_DELEGATE_OneParam(FGetSessionRequestDelegate, const GetSessionRequestResponse&);
    DECLARE_DELEGATE_OneParam(FRevokeSessionDelegate, const RevokeSessionResponse&);
    
    FHttpRequestPtr CreateSessionRequest(const CreateSessionRequestRequest& Request, const FCreateSessionRequestDelegate& Delegate = FCreateSessionRequestDelegate()) const;
    FHttpRequestPtr GetActiveSession(const GetActiveSessionRequest& Request, const FGetActiveSessionDelegate& Delegate = FGetActiveSessionDelegate()) const;
    FHttpRequestPtr GetAllActiveSessions(const GetAllActiveSessionsRequest& Request, const FGetAllActiveSessionsDelegate& Delegate = FGetAllActiveSessionsDelegate()) const;
    FHttpRequestPtr GetSessionRequest(const GetSessionRequestRequest& Request, const FGetSessionRequestDelegate& Delegate = FGetSessionRequestDelegate()) const;
    FHttpRequestPtr RevokeSession(const RevokeSessionRequest& Request, const FRevokeSessionDelegate& Delegate = FRevokeSessionDelegate()) const;
    
private:
    void OnCreateSessionRequestResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FCreateSessionRequestDelegate Delegate) const;
    void OnGetActiveSessionResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetActiveSessionDelegate Delegate) const;
    void OnGetAllActiveSessionsResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetAllActiveSessionsDelegate Delegate) const;
    void OnGetSessionRequestResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetSessionRequestDelegate Delegate) const;
    void OnRevokeSessionResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FRevokeSessionDelegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}
