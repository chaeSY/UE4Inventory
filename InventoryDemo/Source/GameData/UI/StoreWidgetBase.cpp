// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Operation/DragDropSlot.h"
#include "Operation/ButtonDownSlot.h"
#include "ItemSlotWidgetBase.h"
#include "../SYCharacter.h"
#include "../SYUtil.h"

void UStoreWidgetBase::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);

	BindWidget();
	InitStoreItemInfo(1); // Test
	UpdateWidgetSlotAll();
}

void UStoreWidgetBase::OnDragDrop(class UDragDropOperation* InDragDropOp)
{
	if (InDragDropOp->IsA(UDragDropSlot::StaticClass()))
	{
		UDragDropSlot* DragDropOp = Cast<UDragDropSlot>(InDragDropOp);
		if (DragDropOp->ToUINumber == EUINumber::Inventory)
		{
			TryBuyItem(DragDropOp->FromSlotIndex);
		}
	}
}

void UStoreWidgetBase::OnButtonDown(class UButtonDownOperation* InButtonDownOp)
{
	if (InButtonDownOp->IsA(UButtonDownSlot::StaticClass()))
	{
		UButtonDownSlot* ButtonDownOp = Cast<UButtonDownSlot>(InButtonDownOp);
		if (ButtonDownOp->PressedKey == EKeys::RightMouseButton)
		{
			TryBuyItem(ButtonDownOp->SlotIndex);
		}
	}
}

bool UStoreWidgetBase::TryBuyItem(int StoreSlotIndex)
{
	ASYCharacter* Character = GetOwningPlayerPawn<ASYCharacter>();
	if (!Character)
		return false;

	if (!StoreItemInfoList.IsValidIndex(StoreSlotIndex))
		return false;

	const FStoreItemInfo& StoreItemInfo = StoreItemInfoList[StoreSlotIndex];
	return Character->TryBuyItem(StoreItemInfo.ItemKey, StoreItemInfo.Count, StoreItemInfo.Price);
}

void UStoreWidgetBase::InitStoreItemInfo(int InStoreClassID)
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
			ItemSlotWidget->OnHovered.AddUFunction(this, FName("OnMouseOverSlot"));
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
		ChangeStoreBtn->OnClicked.AddDynamic(this,&UStoreWidgetBase::OnClickChangeStoreButton);
	}

	// Bind StoreIDText
	StoreIDText = Cast<UEditableTextBox>(GetWidgetFromName("StoreIDTextBox"));
}

void UStoreWidgetBase::UpdateWidgetSlotAll()
{
	for (int SlotIndex = 0; SlotIndex < MaxStoreSlotCount; ++SlotIndex)
	{
		if (StoreItemInfoList.IsValidIndex(SlotIndex))
		{
			ItemSlotWidgetList[SlotIndex]->UpdateSlot(StoreItemInfoList[SlotIndex]);
		}
		else
		{
			ItemSlotWidgetList[SlotIndex]->UpdateSlot(FItemInfo());
		}
	}
}

void UStoreWidgetBase::OnMouseOverSlot(int SlotIndex)
{
	if (PriceText)
	{
		if (StoreItemInfoList.IsValidIndex(SlotIndex))
		{
			PriceText->SetText(FText::AsNumber(StoreItemInfoList[SlotIndex].Price));
		}
		else
		{
			PriceText->SetText(FText());
		}
	}
}

void UStoreWidgetBase::OnClickChangeStoreButton()
{
	if (StoreIDText)
	{
		int StoreID = FCString::Atoi(*StoreIDText->GetText().ToString());
		if (StoreID != 0)
		{
			InitStoreItemInfo(StoreID);
		}

		UpdateWidgetSlotAll();
	}
}