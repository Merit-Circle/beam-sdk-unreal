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
 * PlayerClientGetAllUsersResponseDataInnerWalletsInner
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientGetAllUsersResponseDataInnerWalletsInner : public Model
{
public:
    virtual ~PlayerClientGetAllUsersResponseDataInnerWalletsInner() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FString Id;
	FDateTime CreatedAt;
	FDateTime UpdatedAt;
	FString ExternalId;
	FString Address;
	int32 ChainId = 0;
	TOptional<FString> ProfileId;
	TOptional<FString> UserId;
};

}
