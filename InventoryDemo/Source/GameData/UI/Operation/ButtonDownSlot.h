// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonDownOperation.h"
#include "ButtonDownSlot.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATA_API UButtonDownSlot : public UButtonDownOperation
{
	GENERATED_BODY()
	
public:
	int SlotIndex = INDEX_NONE;
};
