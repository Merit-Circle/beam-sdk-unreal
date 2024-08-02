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

class PLAYERCLIENT_API PlayerClientConnectorApi
{
public:
	PlayerClientConnectorApi();
	~PlayerClientConnectorApi();

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

	class CreateConnectionRequestRequest;
	class CreateConnectionRequestResponse;
	class GetConnectionRequestRequest;
	class GetConnectionRequestResponse;
	
    DECLARE_DELEGATE_OneParam(FCreateConnectionRequestDelegate, const CreateConnectionRequestResponse&);
    DECLARE_DELEGATE_OneParam(FGetConnectionRequestDelegate, const GetConnectionRequestResponse&);
    
    FHttpRequestPtr CreateConnectionRequest(const CreateConnectionRequestRequest& Request, const FCreateConnectionRequestDelegate& Delegate = FCreateConnectionRequestDelegate()) const;
    FHttpRequestPtr GetConnectionRequest(const GetConnectionRequestRequest& Request, const FGetConnectionRequestDelegate& Delegate = FGetConnectionRequestDelegate()) const;
    
private:
    void OnCreateConnectionRequestResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FCreateConnectionRequestDelegate Delegate) const;
    void OnGetConnectionRequestResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetConnectionRequestDelegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}
