// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(Display = "None"),
	Equipment = 1	UMETA(Display = "Equipment"),
	Consumable = 2	UMETA(Display = "Consumalbe"),
	Etc = 3			UMETA(Display = "Etc"),
};

UENUM(BlueprintType)
enum class ETableType : uint8
{
	None = 0			UMETA(Display = "None"),
	EquipmentItem = 1	UMETA(Display = "EquipmentItem"),
	ConsumableItem = 2	UMETA(Display = "ConsumableItem"),
	EtcItem = 3			UMETA(Display = "EtcItem"),
	Store = 4			UMETA(Display = "Store"),
};

UENUM(BlueprintType)
enum class EUINumber : uint8
{
	None = 0		UMETA(Display = "None"),
	Screen = 1		UMETA(Display = "Screen"),
	Inventory = 2	UMETA(Display = "Inventory"),
	Store = 3		UMETA(Display = "Store"),
};
