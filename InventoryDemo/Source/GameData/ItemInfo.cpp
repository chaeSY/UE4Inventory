// Fill out your copyright notice in the Description page of Project Settings.
#include "ItemInfo.h"
#include "SYUtil.h"

FItemKey::FItemKey()
	:Key(0)
{
}

FItemKey::FItemKey(EItemType InItemType, int32 InItemID)
	: Type(InItemType), ID(InItemID)
{
}

bool FItemKey::operator==(const FItemKey& other) const
{
	return Key == other.Key;
}

ETableType FItemKey::GetTableType()
{
	if (Type == EItemType::Equipment)
	{
		return ETableType::EquipmentItem;
	}
	else if (Type == EItemType::Consumable)
	{
		return ETableType::ConsumableItem;
	}
	else if (Type == EItemType::Etc)
	{
		return ETableType::EtcItem;
	}

	return ETableType::None;
}


bool FItemInfo::IsStackable() const
{
	return MaxStackCount > 1;
}

FStoreItemInfo FItemInfoFactory::CreateEmptyStoreItemInfo()
{
	return FStoreItemInfo();
}

FStoreItemInfo FItemInfoFactory::CreateStoreItemInfo(UObject* WorldContextObject, FItemKey ItemKey, int ItemCount)
{
	FStoreItemInfo StoreItemInfo;
	CreateItemInfo(&StoreItemInfo, WorldContextObject, ItemKey, ItemCount);

	return StoreItemInfo;
}

FInventoryItemInfo FItemInfoFactory::CreateEmptyInventoryItemInfo(int SlotIndex)
{
	FInventoryItemInfo InventoryItemInfo;
	InventoryItemInfo.SlotIndex = SlotIndex;

	return InventoryItemInfo;
}

FInventoryItemInfo FItemInfoFactory::CreateInventoryItemInfo(UObject* WorldContextObject, FItemKey ItemKey, int ItemCount, int SlotIndex)
{
	FInventoryItemInfo InventoryItemInfo;
	CreateItemInfo(&InventoryItemInfo, WorldContextObject, ItemKey, ItemCount);
	InventoryItemInfo.SlotIndex = SlotIndex;

	return InventoryItemInfo;
}

void FItemInfoFactory::CreateItemInfo(FItemInfo * OutItemInfo, UObject* WorldContextObject, FItemKey ItemKey, int ItemCount)
{
	FSYTableItemBase* ItemTable = SYUtil::GetGameData<FSYTableItemBase>(WorldContextObject, ItemKey.GetTableType(), ItemKey.ID);
	if (ItemTable)
	{
		OutItemInfo->ItemKey = ItemKey;
		OutItemInfo->Count = ItemCount;
		OutItemInfo->MaxStackCount = ItemTable->MaxStackCount;
		OutItemInfo->IconTexture = ItemTable->IconTexture;
		OutItemInfo->Price = ItemTable->Price;
	}
}
