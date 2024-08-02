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

#include "PlayerClientUsersApiOperations.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "HttpModule.h"
#include "PlatformHttp.h"

namespace OpenAPI
{

FString PlayerClientUsersApi::GetAllUsersRequest::ComputePath() const
{
	FString Path(TEXT("/v1/player/users"));
	TArray<FString> QueryParams;
	if(Limit.IsSet())
	{
		QueryParams.Add(FString(TEXT("limit=")) + ToUrlString(Limit.GetValue()));
	}
	if(Offset.IsSet())
	{
		QueryParams.Add(FString(TEXT("offset=")) + ToUrlString(Offset.GetValue()));
	}
	Path += TCHAR('?');
	Path += FString::Join(QueryParams, TEXT("&"));

	return Path;
}

void PlayerClientUsersApi::GetAllUsersRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));

}

void PlayerClientUsersApi::GetAllUsersResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT(""));
		break;
	}
}

bool PlayerClientUsersApi::GetAllUsersResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString PlayerClientUsersApi::GetUserRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("entityId"), FStringFormatArg(ToUrlString(EntityId)) } };

	FString Path = FString::Format(TEXT("/v1/player/users/{entityId}"), PathParams);

	return Path;
}

void PlayerClientUsersApi::GetUserRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));

}

void PlayerClientUsersApi::GetUserResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT(""));
		break;
	}
}

bool PlayerClientUsersApi::GetUserResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

}
