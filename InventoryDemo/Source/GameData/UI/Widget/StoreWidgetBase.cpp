// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "UIOperation.h"
#include "ItemSlotWidgetBase.h"
#include "SYCharacter.h"
#include "SYUtil.h"

void UStoreWidgetBase::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);
	BindWidget();
}

void UStoreWidgetBase::BindWidget()
{
	// Bind SlotWidget
	for (int StoreSlotIndex = 0; StoreSlotIndex < MaxStoreSlotCount; ++StoreSlotIndex)
	{
		FString WidgetName = FString::Printf(TEXT("ItemSlot_%d"), StoreSlotIndex);
		UItemSlotWidgetBase* ItemSlotWidget = Cast<UItemSlotWidgetBase>(GetWidgetFromName(*WidgetName));
		if (ItemSlotWidget)
		{
			ItemSlotWidget->SetSlotIndex(StoreSlotIndex);
			ItemSlotWidget->SetParentUINumber(EUINumber::Store);
			ItemSlotWidget->OnHovered.AddUFunction(this, FName("OnMouseOverSlotInternal"));
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
	OnMouseOverInSlot.Broadcast(SlotIndex);
}

void UStoreWidgetBase::OnClickChangeStoreButtonInternal()
{
	if (StoreIDText)
	{
		int StoreID = FCString::Atoi(*StoreIDText->GetText().ToString());
		OnClickedChangeStore.Broadcast(StoreID);
	}
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
