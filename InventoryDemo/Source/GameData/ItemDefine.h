// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefine.generated.h"

enum { INVALID_CLASSID = 0 };

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0		UMETA(Display = "None"),
	Equipment = 1	UMETA(Display = "Equipment"),
	Consumable = 2	UMETA(Display = "Consumalbe"),
	Etc = 3			UMETA(Display = "Etc"),
};

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

	FItemKey() :Key(0) { }
	FItemKey(EItemType InItemType, int32 InItemID) :Type(InItemType), ID(InItemID) { }
	bool operator== (const FItemKey& other) const
	{
		return Key == other.Key;
	}
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
	int Price = 0;
	int MaxStackCount = 0;
	
	static FItemInfo CreateItemInfo(const UObject* WorldContextObject, EItemType ItemType, int ItemID);
	bool   IsStackable() const;
};