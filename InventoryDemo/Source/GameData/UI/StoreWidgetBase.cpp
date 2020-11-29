// Fill out your copyright notice in the Description page of Project Settings.


#include "StoreWidgetBase.h"
#include "ItemSlotWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Operation/DragDropSlot.h"
#include "Operation/ButtonDownSlot.h"
#include "InventoryWidgetBase.h"
#include "../ItemDefine.h"
#include "../SYGameInstance.h"
#include "../GameData/SYGameDataManager.h"
#include "../SYCharacter.h"
#include "../SYPlayerController.h"

void UStoreWidgetBase::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);

	BindWidget();
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

void UStoreWidgetBase::TryBuyItem(int StoreSlotIndex)
{
	ASYPlayerController* SYController = Cast<ASYPlayerController>(GetOwningPlayer());
	if (!SYController)
		return;

	ASYCharacter* Character = Cast<ASYCharacter>(SYController->GetCharacter());
	if (!Character)
		return;

	if (!StoreItemInfoList.IsValidIndex(StoreSlotIndex))
		return;

	const FItemInfo& StoreItemInfo = StoreItemInfoList[StoreSlotIndex];
	if (Character->TryBuyItem(StoreItemInfo))
	{
		if (SYController->WidgetManager)
		{
			UInventoryWidgetBase* InventoryWidget = SYController->WidgetManager->GetWidget<UInventoryWidgetBase>(EUINumber::Inventory);
			if (InventoryWidget)
			{
				InventoryWidget->OnBuyItem();
			}
		}
	}
}

void UStoreWidgetBase::UpdateStoreItemInfo(int InStoreClassID)
{
	StoreItemInfoList.Empty();

	USYGameInstance* GameInstance = Cast<USYGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
		return;

	USYGameDataManager* GameDataManager = GameInstance->GetGameDataManager();
	if (!GameDataManager)
		return;

	FSYTableStore* StoreTable = GameDataManager->GetGameData<FSYTableStore>(ETableType::Store, InStoreClassID);
	if (StoreTable)
	{
		for (int ItemIndex = 0; ItemIndex < StoreTable->StoreItemList.Num(); ++ItemIndex)
		{
			const FItemKeyProperty& ItemKey = StoreTable->StoreItemList[ItemIndex];
			StoreItemInfoList.Add(FItemInfo::CreateItemInfo(GetWorld(), ItemKey.Type, ItemKey.ID));
		}
	}
}

void UStoreWidgetBase::BindWidget()
{
	// Bind SlotWidget
	for (int StoreSlotIndex = 0; StoreSlotIndex < MaxSlotCount; ++StoreSlotIndex)
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
	for (int SlotIndex = 0; SlotIndex < MaxSlotCount; ++SlotIndex)
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
			UpdateStoreItemInfo(StoreID);
		}

		UpdateWidgetSlotAll();
	}
}
