// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SYDefine.h"
#include "UIOperation.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATA_API USYMouseButtonDownOp : public UObject
{
	GENERATED_BODY()

public:
	FKey PressedKey;
};

UCLASS()
class GAMEDATA_API USYSlotMouseButtonDownOp : public USYMouseButtonDownOp
{
	GENERATED_BODY()

public:
	int SlotIndex = INDEX_NONE;
};

UCLASS()
class GAMEDATA_API USYDragDropOp : public UDragDropOperation
{
	GENERATED_BODY()

public:
	EUINumber SrcUINumber;
	EUINumber DstUINumber;
};

UCLASS()
class GAMEDATA_API USYSlotDragDropOp : public USYDragDropOp
{
	GENERATED_BODY()
	
public:
	int SrcSlotIndex;
	int DstSlotIndex;
};