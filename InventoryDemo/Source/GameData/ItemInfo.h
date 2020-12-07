// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDefine.h"
/**
 * 
 */

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