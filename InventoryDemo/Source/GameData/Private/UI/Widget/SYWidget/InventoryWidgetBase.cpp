// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "SYInteractionWidgetItemSlot.h"
#include "SYCharacter.h"
#include "SYUtil.h"

void UInventoryWidgetBase::NativeConstruct()
{
	UINumber = EUINumber::Inventory;
	SetVisibility(ESlateVisibility::Hidden);

	BindWidget();
	UpdateTabColor(INDEX_NONE, EquipmentTab);
	UpdateCash(0);

	for (int SlotIndex = 0; SlotIndex < ItemSlotWidgetList.Num(); ++SlotIndex)
	{
		UpdateSlot(SlotIndex, FInventoryItemInfo::Empty);
	}
}

void UInventoryWidgetBase::BindWidget()
{
	// binding ItemSlotWidgetList
	for (int InvenSlotIdex = 0; InvenSlotIdex < InventorySlotCount; ++InvenSlotIdex)
	{
		FString WidgetName = FString::Printf(TEXT("InventorySlot_%d"), InvenSlotIdex);
		USYInteractionWidgetItemSlot* ItemSlotWidget = Cast<USYInteractionWidgetItemSlot>(GetWidgetFromName(*WidgetName));
		if (ItemSlotWidget)
		{
			ItemSlotWidget->SetSlotIndex(InvenSlotIdex);
			ItemSlotWidget->SetParentUINumber(EUINumber::Inventory);
			ItemSlotWidget->OnDragDrop().AddUFunction(this, FName("OnSlotDragDropInternal"));
			ItemSlotWidget->OnMouseRButtonDown().AddUFunction(this, FName("OnSlotRButtonDownInternal"));
			ItemSlotWidgetList.Add(ItemSlotWidget);
		}
	}

	// binding Tab
	for (int TabIndex = 0; TabIndex < Tab_End; ++TabIndex)
	{
		FString BorderName = FString::Printf(TEXT("TabButton_%d"), TabIndex);
		TabButton[TabIndex] = Cast<UButton>(GetWidgetFromName(*BorderName));
	}

	if (TabButton[EquipmentTab])
		TabButton[EquipmentTab]->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickEquipmentTab);

	if (TabButton[ConsumableTab])
		TabButton[ConsumableTab]->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickConsumableTab);

	if (TabButton[EtcTab])
		TabButton[EtcTab]->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickEtcTab);


	// binding cash
	CashText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CashText")));
	AddCashButton = Cast<UButton>(GetWidgetFromName(TEXT("AddCashButton")));
	if (AddCashButton)
		AddCashButton->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickAddCashInternal);

}

void UInventoryWidgetBase::UpdateSlot(int SlotIndex, const FItemInfo& InItemInfo)
{
	if (ItemSlotWidgetList.IsValidIndex(SlotIndex))
	{
		ItemSlotWidgetList[SlotIndex]->UpdateSlot(InItemInfo);
	}
}

void UInventoryWidgetBase::UpdateTabColor(int PrevTabIndex, int CurrentTabIndex)
{
	if (PrevTabIndex >= 0 && PrevTabIndex < Tab_End)
	{
		FLinearColor DefaultTabColor(1.f, 1.f, 1.f, 0.8f);
		if (TabButton[PrevTabIndex])
		{
			TabButton[PrevTabIndex]->SetColorAndOpacity(DefaultTabColor);
			TabButton[PrevTabIndex]->SetBackgroundColor(DefaultTabColor);
		}
	}

	if (CurrentTabIndex >= 0 && CurrentTabIndex < Tab_End)
	{
		FLinearColor SelectedTabColor(1.f, 0.6f, 0.f, 1.f);
		if (TabButton[CurrentTabIndex])
		{
			TabButton[CurrentTabIndex]->SetColorAndOpacity(SelectedTabColor);
			TabButton[CurrentTabIndex]->SetBackgroundColor(SelectedTabColor);
		}
	}
}

void UInventoryWidgetBase::UpdateCash(int Cash)
{
	if (CashText)
	{
		CashText->SetText(FText::AsNumber(Cash));
	}
}

void UInventoryWidgetBase::OnClickEquipmentTab()
{
	//OnClickTab(EquipmentTab);
}

void UInventoryWidgetBase::OnClickConsumableTab()
{
	//OnClickTab(ConsumableTab);
}

void UInventoryWidgetBase::OnClickEtcTab()
{
	//OnClickTab(EtcTab);
}

void UInventoryWidgetBase::OnClickAddCashInternal()
{
	AddCashButtonDownEvent.Broadcast();
}

void UInventoryWidgetBase::OnSlotDragDropInternal(EUINumber SrcUINumber, int32 InventorySlotIndex, EUINumber DstUINumber, int32 DstSlotIndex)
{
	SlotDragDropEvent.Broadcast(SrcUINumber, InventorySlotIndex, DstUINumber, DstSlotIndex);
}

void UInventoryWidgetBase::OnSlotRButtonDownInternal(int SlotIndex)
{
	SlotRButtonDownEvent.Broadcast(SlotIndex);
}


void UInventoryWidgetBase::OnClickedTabInternal(int TabIndex)
{
	TabButtonDownEvent.Broadcast(TabIndex);
}


FSlotDragDropEvent& UInventoryWidgetBase::OnSlotDragDrop()
{
	return SlotDragDropEvent;
}

FSlotEvent& UInventoryWidgetBase::OnSlotRButtonDown()
{
	return SlotRButtonDownEvent;
}

FTabEvent& UInventoryWidgetBase::OnClickedTab()
{
	return TabButtonDownEvent;
}

FButtonEvent& UInventoryWidgetBase::OnClickeAddCash()
{
	return AddCashButtonDownEvent;
}