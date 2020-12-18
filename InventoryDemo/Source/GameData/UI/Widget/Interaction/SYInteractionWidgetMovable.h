// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYInteractionWidgetBase.h"
#include "SYInteractionWidgetMovable.generated.h"

/**
 * 
 */
class UDragDropOperation;

UCLASS(Blueprintable, BlueprintType)
class GAMEDATA_API USYInteractionWidgetMovable: public USYInteractionWidgetBase
{
	GENERATED_BODY()

protected:
	//virtual void OnDragDrop(UDragDropOperation* InOperation);
	virtual void OnDragging(UDragDropOperation* InOperation);

protected:
};
