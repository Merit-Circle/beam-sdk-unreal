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
#include "PlayerClientCheck200ResponseInfoValue.h"

namespace OpenAPI
{

/*
 * PlayerClientCheck503Response
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientCheck503Response : public Model
{
public:
    virtual ~PlayerClientCheck503Response() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TOptional<FString> Status;
	TOptional<TMap<FString, PlayerClientCheck200ResponseInfoValue>> Info;
	TOptional<TMap<FString, PlayerClientCheck200ResponseInfoValue>> Error;
	TOptional<TMap<FString, PlayerClientCheck200ResponseInfoValue>> Details;
};

}
