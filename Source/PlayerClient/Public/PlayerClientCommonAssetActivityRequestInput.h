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

#include "PlayerClientBaseModel.h"

namespace OpenAPI
{

/*
 * PlayerClientCommonAssetActivityRequestInput
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientCommonAssetActivityRequestInput : public Model
{
public:
    virtual ~PlayerClientCommonAssetActivityRequestInput() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TOptional<double> Limit;
	enum class TypesEnum
	{
		Ask,
		AskCancel,
		Bid,
		BidCancel,
		Sale,
		Mint,
		Transfer,
  	};

	static FString EnumToString(const TypesEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, TypesEnum& EnumValue);
	TOptional<TArray<TypesEnum>> Types;
	TOptional<int64> ChainId;
	TOptional<FString> Continuation;
};

}
