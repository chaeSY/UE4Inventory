// Fill out your copyright notice in the Description page of Project Settings.

#include "SYInteractionWidgetItemSlot.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UIOperation.h"
#include "SYWidgetBase.h"
#include "SYUtil.h"
#include "ItemInfo.h"

void USYInteractionWidgetItemSlot::NativeConstruct()
{
	BindWidget();
}

void USYInteractionWidgetItemSlot::BindWidget()
{
	ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemSlotBorder = Cast<UBorder>(GetWidgetFromName(TEXT("ItemSlotBorder")));
}

void USYInteractionWidgetItemSlot::SetSlotIndex(int InSlotIndex)
{
	SlotIndex = InSlotIndex;
}

void USYInteractionWidgetItemSlot::SetParentUINumber(EUINumber UINumber)
{
	ParentUINumber = UINumber;
}

void USYInteractionWidgetItemSlot::UpdateSlot(const FItemInfo& InItemInfo)
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

USYInteractionWidgetItemSlot::FSlotEvent& USYInteractionWidgetItemSlot::OnMouseOver()
{
	return MouseOverEvent;
}

USYInteractionWidgetItemSlot::FSlotEvent& USYInteractionWidgetItemSlot::OnMouseRButtonDown()
{
	return MouseRButtonDownEvent;
}

USYInteractionWidgetItemSlot::FDragDropEvent& USYInteractionWidgetItemSlot::OnDragDrop()
{
	return DragDropEvent;
}

void USYInteractionWidgetItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ItemSlotBorder)
	{
		ItemSlotBorder->SetBrushFromTexture(BackgroundTextureMouseOver);
	}

	MouseOverEvent.Broadcast(SlotIndex);
}

void USYInteractionWidgetItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (ItemSlotBorder)
	{
		ItemSlotBorder->SetBrushFromTexture(BackgroundTexture);
	}
}

void USYInteractionWidgetItemSlot::OnDragDropInternal(UDragDropOperation * InOperation)
{
	if (!InOperation || !InOperation->Payload)
		return;

	if (InOperation->Payload->IsA(UDragDropPayloadSlot::StaticClass()))
	{
		UDragDropPayloadSlot* payload = Cast<UDragDropPayloadSlot>(InOperation->Payload);
		if (payload)
		{
			DragDropEvent.Broadcast(payload->SrcUINumber, payload->SrcSlotIndex, payload->DstUINumber, payload->DstSlotIndex);
		}
	}
}

void USYInteractionWidgetItemSlot::OnMouseRButtonDownInternal()
{
	MouseRButtonDownEvent.Broadcast(SlotIndex);
}

UDragDropPayloadBase* USYInteractionWidgetItemSlot::CreatePayload()
{
	UDragDropPayloadSlot* Payload = NewObject<UDragDropPayloadSlot>();
	if (Payload)
	{
		Payload->SrcUINumber = ParentUINumber;
		Payload->SrcSlotIndex = SlotIndex;
	}

	return Payload;
}

void USYInteractionWidgetItemSlot::SetPayloadOnDrop(UDragDropPayloadBase * Payload)
{
	if (Payload && Payload->IsA(UDragDropPayloadSlot::StaticClass()))
	{
		UDragDropPayloadSlot* PayloadSlot = Cast<UDragDropPayloadSlot>(Payload);
		PayloadSlot->DstUINumber = ParentUINumber;
		PayloadSlot->DstSlotIndex = SlotIndex;
	}
}

bool USYInteractionWidgetItemSlot::IsShowItemCount(const FItemInfo& InItemInfo)
{
	if (InItemInfo.ItemKey.ID == 0)
		return false;

	if (!InItemInfo.IsStackable())
		return false;

	if (InItemInfo.IsStackable() && InItemInfo.Count == 1)
		return false;

	return true;
}