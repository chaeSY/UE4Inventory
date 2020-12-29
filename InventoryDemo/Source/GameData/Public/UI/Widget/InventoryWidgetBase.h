// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfo.h"
#include "SYWidgetDefine.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */

class USYInteractionWidgetItemSlot;
class UButton;
class UTextBlock;

UCLASS(Blueprintable)
class GAMEDATA_API UInventoryWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()

public:
	enum { InventorySlotCount = 8 };
	enum { EquipmentTab = 0, ConsumableTab = 1, EtcTab = 2, Tab_End };

public:
	FSlotDragDropEvent& OnSlotDragDrop();
	FSlotEvent& OnSlotRButtonDown();
	FTabEvent& OnClickedTab();
	FButtonEvent& OnClickeAddCash();

	void UpdateTabColor(int PrevTabIndex, int CurrentTabIndex);
	void UpdateSlot(int SlotIndex, const FItemInfo& InItemInfo);
	void UpdateCash(int Cash);

private:
	FSlotDragDropEvent	SlotDragDropEvent;
	FSlotEvent			SlotRButtonDownEvent;
	FTabEvent			TabButtonDownEvent;
	FButtonEvent		AddCashButtonDownEvent;

	TArray<USYInteractionWidgetItemSlot*> ItemSlotWidgetList;
	UButton* TabButton[Tab_End];
	UButton* AddCashButton;
	UTextBlock* CashText;

private:
	virtual void NativeConstruct() override;
	void BindWidget();


	UFUNCTION()
	void OnSlotDragDropInternal(EUINumber SrcUINumber, int32 InventorySlotIndex, EUINumber DstUINumber, int32 DstSlotIndex);

	UFUNCTION()
	void OnSlotRButtonDownInternal(int SlotIndex);

	UFUNCTION()
	void OnClickedTabInternal(int TabIndex);

	UFUNCTION()
	void OnClickAddCashInternal();

	// @TODO: tab button 다시 구현
	UFUNCTION()
	void OnClickEquipmentTab();

	UFUNCTION()
	void OnClickConsumableTab();

	UFUNCTION()
	void OnClickEtcTab();

};
