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

#include "PlayerClientConvertTokenRequestInput.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

inline FString ToString(const PlayerClientConvertTokenRequestInput::OperationProcessingEnum& Value)
{
	switch (Value)
	{
	case PlayerClientConvertTokenRequestInput::OperationProcessingEnum::SignOnly:
		return TEXT("SignOnly");
	case PlayerClientConvertTokenRequestInput::OperationProcessingEnum::Execute:
		return TEXT("Execute");
	}

	UE_LOG(LogPlayerClient, Error, TEXT("Invalid PlayerClientConvertTokenRequestInput::OperationProcessingEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString PlayerClientConvertTokenRequestInput::EnumToString(const PlayerClientConvertTokenRequestInput::OperationProcessingEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, PlayerClientConvertTokenRequestInput::OperationProcessingEnum& Value)
{
	static TMap<FString, PlayerClientConvertTokenRequestInput::OperationProcessingEnum> StringToEnum = { 
		{ TEXT("SignOnly"), PlayerClientConvertTokenRequestInput::OperationProcessingEnum::SignOnly },
		{ TEXT("Execute"), PlayerClientConvertTokenRequestInput::OperationProcessingEnum::Execute }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool PlayerClientConvertTokenRequestInput::EnumFromString(const FString& EnumAsString, PlayerClientConvertTokenRequestInput::OperationProcessingEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const PlayerClientConvertTokenRequestInput::OperationProcessingEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, PlayerClientConvertTokenRequestInput::OperationProcessingEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void PlayerClientConvertTokenRequestInput::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("tokenIn")); WriteJsonValue(Writer, TokenIn);
	Writer->WriteIdentifierPrefix(TEXT("tokenOut")); WriteJsonValue(Writer, TokenOut);
	Writer->WriteIdentifierPrefix(TEXT("amountIn")); WriteJsonValue(Writer, AmountIn);
	Writer->WriteIdentifierPrefix(TEXT("amountOut")); WriteJsonValue(Writer, AmountOut);
	if (ReceiverEntityId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("receiverEntityId")); WriteJsonValue(Writer, ReceiverEntityId.GetValue());
	}
	if (ReceiverWalletAddress.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("receiverWalletAddress")); WriteJsonValue(Writer, ReceiverWalletAddress.GetValue());
	}
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

bool PlayerClientConvertTokenRequestInput::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("tokenIn"), TokenIn);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("tokenOut"), TokenOut);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amountIn"), AmountIn);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amountOut"), AmountOut);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("receiverEntityId"), ReceiverEntityId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("receiverWalletAddress"), ReceiverWalletAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("optimistic"), Optimistic);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("sponsor"), Sponsor);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("policyId"), PolicyId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chainId"), ChainId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("operationProcessing"), OperationProcessing);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("operationId"), OperationId);

	return ParseSuccess;
}

}
