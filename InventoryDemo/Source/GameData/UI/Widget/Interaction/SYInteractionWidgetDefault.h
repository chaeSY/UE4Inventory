// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYInteractionWidgetBase.h"
#include "SYDefine.h"
#include "SYInteractionWidgetDefault.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GAMEDATA_API USYInteractionWidgetDefault : public USYInteractionWidgetBase
{
	GENERATED_BODY()

protected:
	virtual auto CreatePayload()->UDragDropPayloadBase* override;
	virtual void SetPayloadOnDrop(UDragDropPayloadBase* Payload) override;
};
