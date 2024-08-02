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

#include "PlayerClientCheck200Response.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

void PlayerClientCheck200Response::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (Status.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("status")); WriteJsonValue(Writer, Status.GetValue());
	}
	if (Info.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("info")); WriteJsonValue(Writer, Info.GetValue());
	}
	if (Error.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("error")); WriteJsonValue(Writer, Error.GetValue());
	}
	if (Details.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("details")); WriteJsonValue(Writer, Details.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool PlayerClientCheck200Response::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("status"), Status);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("info"), Info);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("error"), Error);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("details"), Details);

	return ParseSuccess;
}

}
