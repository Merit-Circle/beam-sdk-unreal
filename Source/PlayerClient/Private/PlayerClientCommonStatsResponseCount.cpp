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

#include "PlayerClientCommonStatsResponseCount.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

void PlayerClientCommonStatsResponseCount::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("tokens")); WriteJsonValue(Writer, Tokens);
	Writer->WriteIdentifierPrefix(TEXT("listed")); WriteJsonValue(Writer, Listed);
	Writer->WriteObjectEnd();
}

bool PlayerClientCommonStatsResponseCount::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("tokens"), Tokens);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("listed"), Listed);

	return ParseSuccess;
}

}
