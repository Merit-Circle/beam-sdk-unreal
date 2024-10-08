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
#include "PlayerClientGetAllUsersResponseDataInner.h"
#include "PlayerClientGetOwnersResponsePagination.h"

namespace OpenAPI
{

/*
 * PlayerClientGetAllUsersResponse
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientGetAllUsersResponse : public Model
{
public:
    virtual ~PlayerClientGetAllUsersResponse() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TArray<PlayerClientGetAllUsersResponseDataInner> Data;
	PlayerClientGetOwnersResponsePagination Pagination;
};

}
