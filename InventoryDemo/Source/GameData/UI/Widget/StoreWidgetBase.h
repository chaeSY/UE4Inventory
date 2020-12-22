// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "ItemInfo.h"
#include "StoreWidgetBase.generated.h"

/**
 * 
 */

class USYInteractionWidgetItemSlot;
class UDragDropOperation;
class USYMouseButtonDownOp;

UCLASS()
class GAMEDATA_API UStoreWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()
	DECLARE_EVENT_OneParam(UStoreWidgetBase, FOnClickChangeStoreButton, int32); // Param1: StoreID
	DECLARE_EVENT_OneParam(UStoreWidgetBase, FSlotEvent, int32);			    // Param1: SlotIndex
	DECLARE_EVENT_FourParams(UStoreWidgetBase, FSlotDragDropEvent, EUINumber, int32, EUINumber, int32);	// Param1: SrcUINumber, Param2: SrcSlotIndex, Param3: DstUINumber, Param4: DstSlotIndex

public:
	enum { MaxStoreSlotCount = 20 };

	void UpdateSlot(int SlotIndex, const FItemInfo& ItemInfo);
	void UpdatePriceText(int Price);

	FOnClickChangeStoreButton& OnClickedChangeStoreButton() {
		return ChangeStoreEvent;
	}

	FSlotEvent& OnMouseOverSlot(){
		return MouseOverSlotEvent;
	}
	
	FSlotEvent& OnMouseRButtonDownSlot() {
		return MouseRButtonDownSlotEvent;
	}

	FSlotDragDropEvent& OnDragDrop2() {
		return DragDropEvent;
	}


private:
	FOnClickChangeStoreButton ChangeStoreEvent;
	FSlotEvent MouseRButtonDownSlotEvent;
	FSlotEvent MouseOverSlotEvent;
	FSlotDragDropEvent DragDropEvent;

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
	UPROPERTY()
	TArray<USYInteractionWidgetItemSlot*> ItemSlotWidgetList;

	UPROPERTY()
	class UTextBlock* PriceText;

	UPROPERTY()
	class UButton* ChangeStoreBtn;

	UPROPERTY()
	class UEditableTextBox* StoreIDText;
};
