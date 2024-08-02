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

#include "PlayerClientCreateOperationRequestInput.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

inline FString ToString(const PlayerClientCreateOperationRequestInput::OperationProcessingEnum& Value)
{
	switch (Value)
	{
	case PlayerClientCreateOperationRequestInput::OperationProcessingEnum::SignOnly:
		return TEXT("SignOnly");
	case PlayerClientCreateOperationRequestInput::OperationProcessingEnum::Execute:
		return TEXT("Execute");
	}

	UE_LOG(LogPlayerClient, Error, TEXT("Invalid PlayerClientCreateOperationRequestInput::OperationProcessingEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString PlayerClientCreateOperationRequestInput::EnumToString(const PlayerClientCreateOperationRequestInput::OperationProcessingEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, PlayerClientCreateOperationRequestInput::OperationProcessingEnum& Value)
{
	static TMap<FString, PlayerClientCreateOperationRequestInput::OperationProcessingEnum> StringToEnum = { 
		{ TEXT("SignOnly"), PlayerClientCreateOperationRequestInput::OperationProcessingEnum::SignOnly },
		{ TEXT("Execute"), PlayerClientCreateOperationRequestInput::OperationProcessingEnum::Execute }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool PlayerClientCreateOperationRequestInput::EnumFromString(const FString& EnumAsString, PlayerClientCreateOperationRequestInput::OperationProcessingEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const PlayerClientCreateOperationRequestInput::OperationProcessingEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, PlayerClientCreateOperationRequestInput::OperationProcessingEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void PlayerClientCreateOperationRequestInput::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("entityId")); WriteJsonValue(Writer, EntityId);
	if (Transactions.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("transactions")); WriteJsonValue(Writer, Transactions.GetValue());
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

bool PlayerClientCreateOperationRequestInput::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("entityId"), EntityId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("transactions"), Transactions);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chainId"), ChainId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("operationProcessing"), OperationProcessing);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("operationId"), OperationId);

	return ParseSuccess;
}

}
