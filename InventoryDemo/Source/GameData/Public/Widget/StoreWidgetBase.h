// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "ItemInfo.h"
#include "SYWidgetDefine.h"
#include "StoreWidgetBase.generated.h"

/**
 * 
 */

class USYInteractionWidgetItemSlot;
class UDragDropOperation;
class USYMouseButtonDownOp;

enum { MaxStoreSlotCount = 20 };

UCLASS()
class GAMEDATA_API UStoreWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()
	DECLARE_EVENT_OneParam(UStoreWidgetBase, FChangeStoreButtonEvent, int32 /* StoreID */);

public:
	FSlotEvent& OnSlotHover();
	FSlotEvent& OnSlotRButtonDown();
	FSlotDragDropEvent& OnSlotDragDrop();
	FChangeStoreButtonEvent& OnButtonClickedChangeStore();

	void UpdateSlot(int SlotIndex, const FItemInfo& ItemInfo);
	void UpdatePriceText(int Price);

private:
	virtual void NativeConstruct() final;
	void BindWidget();

	UFUNCTION()
	void OnMouseOverSlotInternal(int SlotIndex);

	UFUNCTION()
	void OnMouseRButtonDownSlotInternal(int SlotIndex);

	UFUNCTION()
	void OnClickChangeStoreButtonInternal();

	UFUNCTION()
	void OnDragDropInternal(EUINumber SrcUINumber, int32 StoreSlotIndex, EUINumber DstUINumber, int32 DstSlotIndex);

private:
	FChangeStoreButtonEvent ChangeStoreEvent;
	FSlotDragDropEvent SlotDragDropEvent;
	FSlotEvent SlotRButtonDownEvent;
	FSlotEvent SlotHoverEvent;

	UPROPERTY()
	TArray<USYInteractionWidgetItemSlot*> ItemSlotWidgetList;

	UPROPERTY()
	class UTextBlock* PriceText;

	UPROPERTY()
	class UButton* ChangeStoreBtn;

	UPROPERTY()
	class UEditableTextBox* StoreIDText;
};
