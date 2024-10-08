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
 * PlayerClientGetActiveSessionResponse
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientGetActiveSessionResponse : public Model
{
public:
    virtual ~PlayerClientGetActiveSessionResponse() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FString Id;
	bool IsActive = false;
	FDateTime StartTime;
	FDateTime EndTime;
	FString SessionAddress;
};

}
