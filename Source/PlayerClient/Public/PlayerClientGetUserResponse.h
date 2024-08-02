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
#include "PlayerClientGetAllUsersResponseDataInnerWalletsInner.h"
#include "PlayerClientGetTransactionResponseUserEntitiesInner.h"

namespace OpenAPI
{

/*
 * PlayerClientGetUserResponse
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientGetUserResponse : public Model
{
public:
    virtual ~PlayerClientGetUserResponse() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FString ExternalEntityId;
	FString UserId;
	FString GameId;
	TArray<PlayerClientGetAllUsersResponseDataInnerWalletsInner> Wallets;
	TArray<PlayerClientGetTransactionResponseUserEntitiesInner> Entities;
};

}
