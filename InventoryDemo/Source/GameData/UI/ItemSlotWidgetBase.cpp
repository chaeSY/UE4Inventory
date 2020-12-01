// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotWidgetBase.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Operation/DragDropSlot.h"
#include "Operation/ButtonDownSlot.h"
#include "WidgetManager.h"
#include "StoreWidgetBase.h"
#include "InventoryWidgetBase.h"
#include "../SYGameInstance.h"
#include "../SYGameModeBase.h"
#include "../SYPlayerController.h"
#include "../GameData/SYGameDataManager.h"
#include "../GameData/SYDataTable.h"
#include "Kismet/GameplayStatics.h"

void UItemSlotWidgetBase::NativeConstruct()
{
	BindWidget();
}

void UItemSlotWidgetBase::BindWidget()
{
	ItemSlotTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/Texture/SlotButtonImage.SlotButtonImage"), NULL, LOAD_None, NULL);
	ItemSlotTexture_MouseOver = LoadObject<UTexture2D>(NULL, TEXT("/Game/Texture/SlotButtonImage2.SlotButtonImage2"), NULL, LOAD_None, NULL);
	EmptySlotTexture = LoadObject<UTexture2D>(NULL, TEXT("/Game/UITexture/T_UI_Empty.T_UI_Empty"), NULL, LOAD_None, NULL);

	ItemSlotIcon = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemSlotBorder = Cast<UBorder>(GetWidgetFromName(TEXT("ItemSlotBorder")));
}

void UItemSlotWidgetBase::SetSlotIndex(int InSlotIndex)
{
	SlotIndex = InSlotIndex;
}

void UItemSlotWidgetBase::SetParentUINumber(EUINumber UINumber)
{
	ParentUINumber = UINumber;
}

void UItemSlotWidgetBase::UpdateSlot(const FItemInfo& InItemInfo)
{
	USYGameInstance* GameInstance = Cast<USYGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance)
		return;

	USYGameDataManager* GameDataManager = GameInstance->GetGameDataManager();
	if (!GameDataManager)
		return;

	// update item icon
	if (ItemSlotIcon)
	{
		if (InItemInfo.ItemKey.ID == 0)
		{
			ItemSlotIcon->SetBrushFromSoftTexture(EmptySlotTexture);
		}
		else if (InItemInfo.ItemKey.Type == EItemType::Equipment)
		{
			FSYTableEquipmentItem* TableRow = GameDataManager->GetGameData<FSYTableEquipmentItem>(ETableType::EquipmentItem, InItemInfo.ItemKey.ID);
			if (TableRow)
				ItemSlotIcon->SetBrushFromSoftTexture(TableRow->ItemIcon);

		}
		else if (InItemInfo.ItemKey.Type == EItemType::Consumable)
		{
			FSYTableConsumeItem* TableRow = GameDataManager->GetGameData<FSYTableConsumeItem>(ETableType::ConsumableItem, InItemInfo.ItemKey.ID);
			if (TableRow)
				ItemSlotIcon->SetBrushFromSoftTexture(TableRow->ItemIcon);

		}
		else if (InItemInfo.ItemKey.Type == EItemType::Etc)
		{
			FSYTableEtcItem* TableRow = GameDataManager->GetGameData<FSYTableEtcItem>(ETableType::EtcItem, InItemInfo.ItemKey.ID);
			if (TableRow)
				ItemSlotIcon->SetBrushFromSoftTexture(TableRow->ItemIcon);
		}
	}

	// update item count text
	if (ItemCountText)
	{
		if (IsShowItemCount(InItemInfo))
		{
			FString ItemCount = FString::Printf(TEXT("X %d"), InItemInfo.Count);
			ItemCountText->SetText(FText::FromString(ItemCount));
		}
		else
		{
			ItemCountText->SetText(FText());
		}
	}
}

FReply UItemSlotWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}
	OnMouseButtonDown(InMouseEvent);

	return reply.NativeReply;
}

void UItemSlotWidgetBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragDropSlot* DragDropOperation = NewObject<UDragDropSlot>();	
	DragDropOperation->FromSlotIndex = SlotIndex;
	DragDropOperation->FromUINumber = ParentUINumber;

	OutOperation = DragDropOperation;
}

bool UItemSlotWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (InOperation->IsA(UDragDropSlot::StaticClass()))
	{
		UDragDropSlot* DragDropOp = Cast<UDragDropSlot>(InOperation);
		DragDropOp->ToSlotIndex = SlotIndex;
		DragDropOp->ToUINumber = ParentUINumber;

		ASYPlayerController* PC = Cast<ASYPlayerController>(GetOwningPlayer());
		if (PC && PC->WidgetManager)
		{
			PC->WidgetManager->DragDrop(DragDropOp);
		}
	}

	return false;
}

void UItemSlotWidgetBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ItemSlotBorder)
	{
		ItemSlotBorder->SetBrushFromTexture(ItemSlotTexture_MouseOver);
	}

	OnHovered.Broadcast(SlotIndex);
}

void UItemSlotWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (ItemSlotBorder)
	{
		ItemSlotBorder->SetBrushFromTexture(ItemSlotTexture);
	}
}

bool UItemSlotWidgetBase::IsShowItemCount(const FItemInfo& InItemInfo)
{
	if (InItemInfo.ItemKey.ID == 0)
		return false;

	if (!InItemInfo.IsStackable())
		return false;

	if (InItemInfo.IsStackable() && InItemInfo.Count == 1)
		return false;

	return true;
}

void UItemSlotWidgetBase::OnMouseButtonDown(const FPointerEvent& InMouseEvent)
{
	ASYPlayerController* PC = Cast<ASYPlayerController>(GetOwningPlayer());
	if (PC && PC->WidgetManager)
	{
		USYWidgetBase* Widget = PC->WidgetManager->GetWidget(ParentUINumber);
		if (Widget)
		{
			UButtonDownSlot* MouseButtonDownOp = NewObject<UButtonDownSlot>();
			if (MouseButtonDownOp)
			{
				MouseButtonDownOp->SlotIndex = SlotIndex;

				if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
				{
					MouseButtonDownOp->PressedKey = EKeys::LeftMouseButton;
				}
				else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
				{
					MouseButtonDownOp->PressedKey = EKeys::RightMouseButton;
				}
				Widget->OnButtonDown(MouseButtonDownOp);
			}
		}
	}
}