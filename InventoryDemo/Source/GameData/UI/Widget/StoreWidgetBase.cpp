// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UIOperation.h"
#include "SYInteractionWidgetItemSlot.h"
#include "SYCharacter.h"
#include "SYUtil.h"

void UStoreWidgetBase::NativeConstruct()
{
	UINumber = EUINumber::Store;
	SetVisibility(ESlateVisibility::Hidden);
	BindWidget();
}

void UStoreWidgetBase::BindWidget()
{
	// Bind SlotWidget
	for (int StoreSlotIndex = 0; StoreSlotIndex < MaxStoreSlotCount; ++StoreSlotIndex)
	{
		FString WidgetName = FString::Printf(TEXT("ItemSlot_%d"), StoreSlotIndex);
		USYInteractionWidgetItemSlot* ItemSlotWidget = Cast<USYInteractionWidgetItemSlot>(GetWidgetFromName(*WidgetName));
		if (ItemSlotWidget)
		{
			ItemSlotWidget->SetSlotIndex(StoreSlotIndex);
			ItemSlotWidget->SetParentUINumber(EUINumber::Store);
			ItemSlotWidget->OnMouseOver().AddUFunction(this, FName("OnMouseOverSlotInternal"));
			ItemSlotWidget->OnDragDrop().AddUFunction(this, FName("OnDragDropInternal"));
			ItemSlotWidget->OnMouseRButtonDown().AddUFunction(this, FName("OnMouseRButtonDownSlotInternal"));

			ItemSlotWidgetList.Add(ItemSlotWidget);
		}
	}
	
	// Bind PriceText
	PriceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PriceText")));
	if (PriceText)
	{
		PriceText->SetText(FText());
	}

	// Bind ChangeStoreButton
	ChangeStoreBtn = Cast<UButton>(GetWidgetFromName("ChangeStoreButton"));
	if (ChangeStoreBtn)
	{
		ChangeStoreBtn->OnClicked.AddDynamic(this,&UStoreWidgetBase::OnClickChangeStoreButtonInternal);
	}

	// Bind StoreIDText
	StoreIDText = Cast<UEditableTextBox>(GetWidgetFromName("StoreIDTextBox"));
}

void UStoreWidgetBase::OnMouseOverSlotInternal(int SlotIndex)
{
	MouseOverSlotEvent.Broadcast(SlotIndex);
}

void UStoreWidgetBase::OnMouseRButtonDownSlotInternal(int SlotIndex)
{
	MouseRButtonDownSlotEvent.Broadcast(SlotIndex);
}

void UStoreWidgetBase::OnClickChangeStoreButtonInternal()
{
	if (StoreIDText)
	{
		int StoreID = FCString::Atoi(*StoreIDText->GetText().ToString());
		ChangeStoreEvent.Broadcast(StoreID);
	}
}

void UStoreWidgetBase::OnDragDropInternal(EUINumber SrcUINumber, int32 StoreSlotIndex, EUINumber DstUINumber, int32 DstSlotIndex)
{
	DragDropEvent.Broadcast(SrcUINumber, StoreSlotIndex, DstUINumber, DstSlotIndex);
}

void UStoreWidgetBase::UpdatePriceText(int Price)
{
	if (PriceText)
	{
		PriceText->SetText(FText::AsNumber(Price));
	}
}

void UStoreWidgetBase::UpdateSlot(int SlotIndex, const FItemInfo& ItemInfo)
{
	if (ItemSlotWidgetList.IsValidIndex(SlotIndex))
	{
		ItemSlotWidgetList[SlotIndex]->UpdateSlot(ItemInfo);
	}
}
