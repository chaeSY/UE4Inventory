// Fill out your copyright notice in the Description page of Project Settings.


#include "SYUIStore.h"
#include "StoreWidgetBase.h"
#include "SYUtil.h"
#include "SYDefine.h"
#include "SYCharacter.h"

void USYUIStore::Init(EUINumber InUINumber, USYWidgetBase* InWidget)
{
	Super::Init(InUINumber, InWidget);
	BindWidget();
}

void USYUIStore::BindWidget()
{
	StoreWidget = Cast<UStoreWidgetBase>(Widget);
	if (StoreWidget)
	{
		StoreWidget->OnSlotDragDrop().AddUFunction(this, FName("OnDragDropSlot"));
		StoreWidget->OnSlotHover().AddUFunction(this, FName("OnMouseOverInSlot"));
		StoreWidget->OnSlotRButtonDown().AddUFunction(this, FName("OnMouseRButtonDownSlot"));
		StoreWidget->OnButtonClickedChangeStore().AddUFunction(this, FName("OnClickedChangeStoreButton"));
	}

	//test
	OnClickedChangeStoreButton(1);
}

void USYUIStore::OnDragDropSlot(EUINumber SrcUINumber, int32 StoreSlotIndex, EUINumber DstUINumber, int32 DstSlotIndex)
{
	if (DstUINumber == EUINumber::Inventory)
	{
		TryBuyItem(StoreSlotIndex);
	}
}

void USYUIStore::OnMouseRButtonDownSlot(int SlotIndex)
{
	TryBuyItem(SlotIndex);
}

bool USYUIStore::TryBuyItem(int StoreSlotIndex)
{
	if (!StoreItemInfoList.IsValidIndex(StoreSlotIndex))
		return false;

	ASYCharacter* Character = SYUtil::GetCharacter(GetWorld());
	if (!Character)
		return false;

	const FStoreItemInfo& StoreItemInfo = StoreItemInfoList[StoreSlotIndex];
	return Character->TryBuyItem(StoreItemInfo.ItemKey, StoreItemInfo.Count, StoreItemInfo.Price);
}

void USYUIStore::InitStoreItemInfo(int InStoreClassID)
{
	StoreItemInfoList.Empty();

	FSYTableStore* StoreTable = SYUtil::GetGameData<FSYTableStore>(GetWorld(), ETableType::Store, InStoreClassID);
	if (StoreTable)
	{
		for (int ItemIndex = 0; ItemIndex < StoreTable->StoreItemList.Num(); ++ItemIndex)
		{
			FItemKey ItemKey = StoreTable->StoreItemList[ItemIndex];
			FStoreItemInfo StoreItemInfo = FItemInfoFactory::CreateStoreItemInfo(GetWorld(), ItemKey, 1);
			StoreItemInfoList.Add(StoreItemInfo);
		}
	}
}

void USYUIStore::OnMouseOverInSlot(int SlotIndex)
{
	if(!StoreWidget)
		return;

	int Price = StoreItemInfoList.IsValidIndex(SlotIndex) ? StoreItemInfoList[SlotIndex].Price : 0;
	StoreWidget->UpdatePriceText(Price);
}

void USYUIStore::OnClickedChangeStoreButton(int StoreID)
{
	if (!StoreWidget)
		return;

	InitStoreItemInfo(StoreID);

	for (int SlotIndex = 0; SlotIndex < MaxStoreSlotCount; ++SlotIndex)
	{
		if (StoreItemInfoList.IsValidIndex(SlotIndex))
		{
			StoreWidget->UpdateSlot(SlotIndex, StoreItemInfoList[SlotIndex]);
		}
		else
		{
			StoreWidget->UpdateSlot(SlotIndex, FStoreItemInfo::Empty);
		}
	}
}
