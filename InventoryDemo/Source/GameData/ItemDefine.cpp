// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefine.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/SYGameDataManager.h"
#include "SYGameInstance.h"

ETableType ConvertItemTypeToTableType(EItemType ItemType)
{
	ETableType TableType = ETableType::None;
	if (ItemType == EItemType::Equipment)
	{
		TableType = ETableType::EquipmentItem;
	}
	else if (ItemType == EItemType::Consumable)
	{
		TableType = ETableType::ConsumableItem;
	}
	else if (ItemType == EItemType::Etc)
	{
		TableType = ETableType::EtcItem;
	}
	
	return TableType;
}

FItemInfo FItemInfo::CreateItemInfo(const UObject* WorldContextObject, EItemType ItemType, int ItemID)
{
	FItemInfo ItemInfo;
	USYGameInstance* GameInstance = Cast<USYGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GameInstance)
		return ItemInfo;

	USYGameDataManager* GameDataManager = GameInstance->GetGameDataManager();
	if (!GameDataManager)
		return ItemInfo;

	ETableType TableType = ConvertItemTypeToTableType(ItemType);
	FSYTableItemBase* TableRow = GameDataManager->GetGameData<FSYTableItemBase>(TableType, ItemID);
	if (TableRow)
	{
		ItemInfo.ItemKey.Type = ItemType;
		ItemInfo.ItemKey.ID = ItemID;
		ItemInfo.Count = 1;
		ItemInfo.MaxStackCount = TableRow->MaxStackCount;
		ItemInfo.Price = TableRow->Price;
		ItemInfo.IconTexture = TableRow->IconTexture;
	}

	return ItemInfo;
}

bool FItemInfo::IsStackable() const
{
	return MaxStackCount > 1;
}
