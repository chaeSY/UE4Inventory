// Fill out your copyright notice in the Description page of Project Settings.


#include "SYUIStore.h"
#include "StoreWidgetBase.h"
#include "UIOperation.h"
#include "SYUtil.h"
#include "SYDefine.h"
#include "SYCharacter.h"

void USYUIStore::Bind()
{
	Widget = SYUtil::GetWidget(this, EUINumber::Store);
	if (Widget)
	{
		Widget->OnDragDropEvent.AddUFunction(this, FName("OnDragDrop"));
		Widget->OnMouseButtonDownEvent.AddUFunction(this, FName("OnMouseButtonDown"));
	}

	StoreWidget = Cast<UStoreWidgetBase>(Widget);
	if (StoreWidget)
	{
		StoreWidget->OnMouseOverInSlot.AddUFunction(this, FName("OnMouseOverInSlot"));
		StoreWidget->OnClickedChangeStore.AddUFunction(this, FName("OnClickedChangeStoreButton"));
	}

	//test
	OnClickedChangeStoreButton(1);
}

void USYUIStore::OnDragDrop(UDragDropOperation* DragDropOp)
{
	if (DragDropOp->IsA(USYSlotDragDropOp::StaticClass()))
	{
		USYSlotDragDropOp* op = Cast<USYSlotDragDropOp>(DragDropOp);
		if (op->DstUINumber == EUINumber::Inventory)
		{
			TryBuyItem(op->SrcSlotIndex);
		}
	}
}

void USYUIStore::OnMouseButtonDown(USYMouseButtonDownOp* MouseButtonDownOp)
{
	if (MouseButtonDownOp->IsA(USYSlotMouseButtonDownOp::StaticClass()))
	{
		USYSlotMouseButtonDownOp* ButtonDownOp = Cast<USYSlotMouseButtonDownOp>(MouseButtonDownOp);
		if (ButtonDownOp->PressedKey == EKeys::RightMouseButton)
		{
			TryBuyItem(ButtonDownOp->SlotIndex);
		}
	}
}

bool USYUIStore::TryBuyItem(int StoreSlotIndex)
{
	// HACK?
	ASYCharacter* Character = Cast<ASYCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!Character)
		return false;

	if (!StoreItemInfoList.IsValidIndex(StoreSlotIndex))
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
	InitStoreItemInfo(StoreID);

	for (int SlotIndex = 0; SlotIndex < UStoreWidgetBase::MaxStoreSlotCount; ++SlotIndex)
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
