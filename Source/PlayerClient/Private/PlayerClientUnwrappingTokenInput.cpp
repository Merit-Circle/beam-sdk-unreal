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

#include "PlayerClientUnwrappingTokenInput.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

inline FString ToString(const PlayerClientUnwrappingTokenInput::OperationProcessingEnum& Value)
{
	switch (Value)
	{
	case PlayerClientUnwrappingTokenInput::OperationProcessingEnum::SignOnly:
		return TEXT("SignOnly");
	case PlayerClientUnwrappingTokenInput::OperationProcessingEnum::Execute:
		return TEXT("Execute");
	}

	UE_LOG(LogPlayerClient, Error, TEXT("Invalid PlayerClientUnwrappingTokenInput::OperationProcessingEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString PlayerClientUnwrappingTokenInput::EnumToString(const PlayerClientUnwrappingTokenInput::OperationProcessingEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, PlayerClientUnwrappingTokenInput::OperationProcessingEnum& Value)
{
	static TMap<FString, PlayerClientUnwrappingTokenInput::OperationProcessingEnum> StringToEnum = { 
		{ TEXT("SignOnly"), PlayerClientUnwrappingTokenInput::OperationProcessingEnum::SignOnly },
		{ TEXT("Execute"), PlayerClientUnwrappingTokenInput::OperationProcessingEnum::Execute }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool PlayerClientUnwrappingTokenInput::EnumFromString(const FString& EnumAsString, PlayerClientUnwrappingTokenInput::OperationProcessingEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const PlayerClientUnwrappingTokenInput::OperationProcessingEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, PlayerClientUnwrappingTokenInput::OperationProcessingEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void PlayerClientUnwrappingTokenInput::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("amount")); WriteJsonValue(Writer, Amount);
	if (Optimistic.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("optimistic")); WriteJsonValue(Writer, Optimistic.GetValue());
	}
	if (Sponsor.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("sponsor")); WriteJsonValue(Writer, Sponsor.GetValue());
	}
	if (PolicyId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("policyId")); WriteJsonValue(Writer, PolicyId.GetValue());
	}
	if (ChainId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("chainId")); WriteJsonValue(Writer, ChainId.GetValue());
	}
	if (OperationProcessing.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("operationProcessing")); WriteJsonValue(Writer, OperationProcessing.GetValue());
	}
	if (OperationId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("operationId")); WriteJsonValue(Writer, OperationId.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool PlayerClientUnwrappingTokenInput::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amount"), Amount);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("optimistic"), Optimistic);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("sponsor"), Sponsor);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("policyId"), PolicyId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chainId"), ChainId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("operationProcessing"), OperationProcessing);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("operationId"), OperationId);

	return ParseSuccess;
}

}
