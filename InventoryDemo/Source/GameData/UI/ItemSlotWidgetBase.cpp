// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotWidgetBase.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Operation/DragDropSlot.h"
#include "Operation/ButtonDownSlot.h"
#include "SYWidgetBase.h"
#include "../SYUtil.h"
#include "../ItemInfo.h"

void UItemSlotWidgetBase::NativeConstruct()
{
	BindWidget();
}

void UItemSlotWidgetBase::BindWidget()
{
	ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
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
	// update item icon
	if (ItemIconImage)
	{
		if (InItemInfo.ItemKey.ID == 0)
		{
			ItemIconImage->SetBrushFromSoftTexture(EmptyItemIconTexture);
		}
		else //
		{
			ItemIconImage->SetBrushFromSoftTexture(InItemInfo.IconTexture);
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

		UWidgetManager* WidgetManager = SYUtil::GetWidgetManager(GetWorld());
		if (WidgetManager)
		{
			WidgetManager->DragDrop(DragDropOp);
		}
	}

	return false;
}

void UItemSlotWidgetBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ItemSlotBorder)
	{
		ItemSlotBorder->SetBrushFromTexture(BackgroundTextureMouseOver);
	}

	OnHovered.Broadcast(SlotIndex);
}

void UItemSlotWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (ItemSlotBorder)
	{
		ItemSlotBorder->SetBrushFromTexture(BackgroundTexture);
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
	USYWidgetBase* Widget = SYUtil::GetWidget(GetWorld(), ParentUINumber);
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