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

#include "PlayerClientGenerateSessionUrlRequestInput.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

void PlayerClientGenerateSessionUrlRequestInput::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("address")); WriteJsonValue(Writer, Address);
	if (SuggestedExpiry.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("suggestedExpiry")); WriteJsonValue(Writer, SuggestedExpiry.GetValue());
	}
	if (ChainId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("chainId")); WriteJsonValue(Writer, ChainId.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool PlayerClientGenerateSessionUrlRequestInput::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("address"), Address);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("suggestedExpiry"), SuggestedExpiry);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chainId"), ChainId);

	return ParseSuccess;
}

}
