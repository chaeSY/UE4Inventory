// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../ItemDefine.h"
#include "SYDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct GAMEDATA_API FSYTableBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Name;
};

USTRUCT(BlueprintType)
struct GAMEDATA_API FSYTableItemBase : public FSYTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::None;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSoftObjectPtr<class UTexture2D> ItemIcon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int MaxStackCount = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int Price = 0;
};

USTRUCT(BlueprintType)
struct GAMEDATA_API FSYTableEquipmentItem : public FSYTableItemBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSYTableEquipmentItem()
	{
		ItemType = EItemType::Equipment;
	}
};

USTRUCT(BlueprintType)
struct GAMEDATA_API FSYTableConsumeItem : public FSYTableItemBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSYTableConsumeItem()
	{
		ItemType = EItemType::Consumable;
	}

};

USTRUCT(BlueprintType)
struct GAMEDATA_API FSYTableEtcItem : public FSYTableItemBase
{
	GENERATED_USTRUCT_BODY()

public:
	FSYTableEtcItem()
	{
		ItemType = EItemType::Etc;
	}
};

USTRUCT(BlueprintType)
struct GAMEDATA_API FSYTableStore : public FSYTableBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TArray<FItemKeyProperty> StoreItemList;
};

