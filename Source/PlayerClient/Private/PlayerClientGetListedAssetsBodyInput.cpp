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

#include "PlayerClientGetListedAssetsBodyInput.h"

#include "PlayerClientModule.h"
#include "PlayerClientHelpers.h"

#include "Templates/SharedPointer.h"

namespace OpenAPI
{

inline FString ToString(const PlayerClientGetListedAssetsBodyInput::SortByEnum& Value)
{
	switch (Value)
	{
	case PlayerClientGetListedAssetsBodyInput::SortByEnum::CreatedAt:
		return TEXT("createdAt");
	case PlayerClientGetListedAssetsBodyInput::SortByEnum::UpdatedAt:
		return TEXT("updatedAt");
	case PlayerClientGetListedAssetsBodyInput::SortByEnum::Price:
		return TEXT("price");
	}

	UE_LOG(LogPlayerClient, Error, TEXT("Invalid PlayerClientGetListedAssetsBodyInput::SortByEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString PlayerClientGetListedAssetsBodyInput::EnumToString(const PlayerClientGetListedAssetsBodyInput::SortByEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, PlayerClientGetListedAssetsBodyInput::SortByEnum& Value)
{
	static TMap<FString, PlayerClientGetListedAssetsBodyInput::SortByEnum> StringToEnum = { 
		{ TEXT("createdAt"), PlayerClientGetListedAssetsBodyInput::SortByEnum::CreatedAt },
		{ TEXT("updatedAt"), PlayerClientGetListedAssetsBodyInput::SortByEnum::UpdatedAt },
		{ TEXT("price"), PlayerClientGetListedAssetsBodyInput::SortByEnum::Price }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool PlayerClientGetListedAssetsBodyInput::EnumFromString(const FString& EnumAsString, PlayerClientGetListedAssetsBodyInput::SortByEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const PlayerClientGetListedAssetsBodyInput::SortByEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, PlayerClientGetListedAssetsBodyInput::SortByEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

inline FString ToString(const PlayerClientGetListedAssetsBodyInput::SortDirectionEnum& Value)
{
	switch (Value)
	{
	case PlayerClientGetListedAssetsBodyInput::SortDirectionEnum::Asc:
		return TEXT("asc");
	case PlayerClientGetListedAssetsBodyInput::SortDirectionEnum::Desc:
		return TEXT("desc");
	}

	UE_LOG(LogPlayerClient, Error, TEXT("Invalid PlayerClientGetListedAssetsBodyInput::SortDirectionEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString PlayerClientGetListedAssetsBodyInput::EnumToString(const PlayerClientGetListedAssetsBodyInput::SortDirectionEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, PlayerClientGetListedAssetsBodyInput::SortDirectionEnum& Value)
{
	static TMap<FString, PlayerClientGetListedAssetsBodyInput::SortDirectionEnum> StringToEnum = { 
		{ TEXT("asc"), PlayerClientGetListedAssetsBodyInput::SortDirectionEnum::Asc },
		{ TEXT("desc"), PlayerClientGetListedAssetsBodyInput::SortDirectionEnum::Desc }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool PlayerClientGetListedAssetsBodyInput::EnumFromString(const FString& EnumAsString, PlayerClientGetListedAssetsBodyInput::SortDirectionEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const PlayerClientGetListedAssetsBodyInput::SortDirectionEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, PlayerClientGetListedAssetsBodyInput::SortDirectionEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void PlayerClientGetListedAssetsBodyInput::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (AssetAddresses.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("assetAddresses")); WriteJsonValue(Writer, AssetAddresses.GetValue());
	}
	if (SortBy.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("sortBy")); WriteJsonValue(Writer, SortBy.GetValue());
	}
	if (SortDirection.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("sortDirection")); WriteJsonValue(Writer, SortDirection.GetValue());
	}
	if (Limit.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("limit")); WriteJsonValue(Writer, Limit.GetValue());
	}
	if (ChainId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("chainId")); WriteJsonValue(Writer, ChainId.GetValue());
	}
	if (Continuation.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("continuation")); WriteJsonValue(Writer, Continuation.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool PlayerClientGetListedAssetsBodyInput::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("assetAddresses"), AssetAddresses);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("sortBy"), SortBy);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("sortDirection"), SortDirection);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("limit"), Limit);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chainId"), ChainId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("continuation"), Continuation);

	return ParseSuccess;
}

}
