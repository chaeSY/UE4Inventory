// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "../WidgetManager.h"
#include "DragDropSlot.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATA_API UDragDropSlot : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	EUINumber FromUINumber;
	int FromSlotIndex;

	EUINumber ToUINumber;
	int ToSlotIndex;
};
