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
#include "PlayerClientGetAssetListingsResponseDataInner.h"

namespace OpenAPI
{

/*
 * PlayerClientGetAssetListingsResponse
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientGetAssetListingsResponse : public Model
{
public:
    virtual ~PlayerClientGetAssetListingsResponse() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TArray<PlayerClientGetAssetListingsResponseDataInner> Data;
	TOptional<FString> Continuation;
};

}
