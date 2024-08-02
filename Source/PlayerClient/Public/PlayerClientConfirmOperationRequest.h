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
#include "PlayerClientConfirmOperationRequestTransactionsInner.h"

namespace OpenAPI
{

/*
 * PlayerClientConfirmOperationRequest
 *
 * 
 */
class PLAYERCLIENT_API PlayerClientConfirmOperationRequest : public Model
{
public:
    virtual ~PlayerClientConfirmOperationRequest() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	enum class StatusEnum
	{
		_Signed,
		Pending,
		Rejected,
		Executed,
		Error,
  	};

	static FString EnumToString(const StatusEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, StatusEnum& EnumValue);
	StatusEnum Status;
	TOptional<TArray<PlayerClientConfirmOperationRequestTransactionsInner>> Transactions;
};

}
