// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfo.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */

class UDragDropOperation;
class USYInteractionWidgetItemSlot;

UCLASS(Blueprintable)
class GAMEDATA_API UInventoryWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()


public:
	enum { InventorySlotCount = 8 };
	enum { EquipmentTab = 0, ConsumableTab = 1, EtcTab = 2, Tab_End };
	void OnBuyItem();

public:
	void UpdateTabColor(int PrevTabIndex, int CurrentTabIndex); // ok -> public
	void UpdateSlot(int SlotIndex, const FItemInfo& InItemInfo); //ok ->public
	void UpdateCash(int Cash); // ok -> public


private:
	virtual void NativeConstruct() override;
	void BindWidget(); //ok

	TArray<USYInteractionWidgetItemSlot*> ItemSlotWidgetList;
	class UButton* TabButton[Tab_End];
	class UTextBlock* CashText;
	class UButton* AddCashButton;

private:
	UFUNCTION()
	void OnClickEquipmentTab();
	
	UFUNCTION()
	void OnClickConsumableTab();
	
	UFUNCTION()
	void OnClickEtcTab();

public:
	DECLARE_EVENT_FourParams(UStoreWidgetBase, FSlotDragDropEvent, EUINumber, int32, EUINumber, int32);	// Param1: SrcUINumber, Param2: SrcSlotIndex, Param3: DstUINumber, Param4: DstSlotIndex
	DECLARE_EVENT_OneParam(UStoreWidgetBase, FSlotEvent, int32); // Param1: SlotIndex
	DECLARE_EVENT_OneParam(UStoreWidgetBase, FTabEvent, int32); // Param1: TabIndex
	DECLARE_EVENT(UStoreWidgetBase, FButtonDownEvent);

	FSlotDragDropEvent& OnSlotDragDrop() {
		return SlotDragDropEvent;
	}

	FSlotEvent& OnSlotRButtonDown() {
		return SlotRButtonDownEvent;
	}

	FTabEvent& OnClickedTab(){
		return TabButtonDownEvent;
	}

	FButtonDownEvent& OnClickeAddCash(){
		return AddCashButtonDownEvent;
	}
	
	FSlotDragDropEvent	SlotDragDropEvent;
	FSlotEvent			SlotRButtonDownEvent;
	FTabEvent			TabButtonDownEvent;
	FButtonDownEvent	AddCashButtonDownEvent;

	UFUNCTION()
	void OnSlotDragDropInternal(EUINumber SrcUINumber, int32 InventorySlotIndex, EUINumber DstUINumber, int32 DstSlotIndex);

	UFUNCTION()
	void OnSlotRButtonDownInternal(int SlotIndex);

	UFUNCTION()
	void OnClickedTabInternal(int TabIndex);

	UFUNCTION()
	void OnClickAddCashInternal();
};
