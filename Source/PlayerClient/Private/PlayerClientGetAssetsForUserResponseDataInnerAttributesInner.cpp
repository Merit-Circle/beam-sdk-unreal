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

#include "PlayerClientGetAssetsForUserResponseDataInnerAttributesInner.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

void PlayerClientGetAssetsForUserResponseDataInnerAttributesInner::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (DisplayType.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("displayType")); WriteJsonValue(Writer, DisplayType.GetValue());
	}
	if (MaxValue.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("maxValue")); WriteJsonValue(Writer, MaxValue.GetValue());
	}
	if (Rarity.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("rarity")); WriteJsonValue(Writer, Rarity.GetValue());
	}
	if (Type.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type.GetValue());
	}
	if (Value.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("value")); WriteJsonValue(Writer, Value.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool PlayerClientGetAssetsForUserResponseDataInnerAttributesInner::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("displayType"), DisplayType);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("maxValue"), MaxValue);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("rarity"), Rarity);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("value"), Value);

	return ParseSuccess;
}

}
