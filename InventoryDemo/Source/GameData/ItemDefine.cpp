// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDefine.h"
#include "Kismet/GameplayStatics.h"
#include "SYGameInstance.h"
#include "SYUtil.h"


FItemKey::FItemKey()
	:Key(0)
{
}

FItemKey::FItemKey(EItemType InItemType, int32 InItemID)
	:Type(InItemType), ID(InItemID)
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