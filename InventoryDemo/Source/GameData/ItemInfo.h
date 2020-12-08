// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYDefine.h"
#include "ItemInfo.generated.h"

/**
 * 
 */

struct FItemKey
{
	union
	{
		int64 Key;
		struct
		{
			EItemType Type;
			int32 ID;
		};
	};

	FItemKey();
	FItemKey(EItemType InItemType, int32 InItemID);
	bool operator== (const FItemKey& other) const;
	ETableType GetTableType();
};

FORCEINLINE uint32 GetTypeHash(const FItemKey& ItemKey)
{
	uint32 Hash = FCrc::MemCrc32(&ItemKey, sizeof(FItemKey));
	return Hash;
}

USTRUCT(BlueprintType)
struct GAMEDATA_API FItemKeyProperty
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	EItemType Type = EItemType::None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 ID = 0;

	operator FItemKey()
	{
		return FItemKey(Type, ID);
	}
};

struct FItemInfo
{
	FItemKey ItemKey;
	int	Count = 0;
	int MaxStackCount = 0;
	int Price = 0;
	TSoftObjectPtr<class UTexture2D> IconTexture;

	bool IsStackable() const;
};

struct FStoreItemInfo : public FItemInfo
{

};

struct FInventoryItemInfo : public FItemInfo
{
	int SlotIndex = INDEX_NONE;
	const static FInventoryItemInfo Empty;
};

class FItemInfoFactory
{
	friend struct FItemInfo;
	friend struct FStoreItemInfo;
	friend struct FInventoryItemInfo;

public:
	static FStoreItemInfo CreateEmptyStoreItemInfo();
	static FStoreItemInfo CreateStoreItemInfo(UObject* WorldContextObject, FItemKey ItemKey, int ItemCount);

	static FInventoryItemInfo CreateEmptyInventoryItemInfo(int SlotIndex);
	static FInventoryItemInfo CreateInventoryItemInfo(UObject* WorldContextObject, FItemKey ItemKey, int ItemCount, int SlotIndex);

private:
	static void CreateItemInfo(FItemInfo * OutItemInfo, UObject* WorldContextObject, FItemKey ItemKey, int ItemCount);
};