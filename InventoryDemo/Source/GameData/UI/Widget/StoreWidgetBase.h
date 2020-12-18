// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "ItemInfo.h"
#include "ItemSlotWidgetBase.h"
#include "StoreWidgetBase.generated.h"

/**
 * 
 */

class UItemSlotWidgetBase;
class UDragDropOperation;
class USYMouseButtonDownOp;

UCLASS()
class GAMEDATA_API UStoreWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()

public:
	enum { MaxStoreSlotCount = 20 };

	void UpdateSlot(int SlotIndex, const FItemInfo& ItemInfo);
	void UpdatePriceText(int Price);

	DECLARE_EVENT_OneParam(UStoreWidgetBase, FChangeStoreButtonClickEvent, int32); // Param1: StoreID
	FChangeStoreButtonClickEvent OnClickedChangeStore;

	DECLARE_EVENT_OneParam(UStoreWidgetBase, FMouseOverSlotEvent, int32); // Param1: SlotIndex
	FMouseOverSlotEvent OnMouseOverInSlot;

private:
	virtual void NativeConstruct() final;
	void BindWidget();

	UFUNCTION()
	void OnMouseOverSlotInternal(int SlotIndex);

	UFUNCTION()
	void OnClickChangeStoreButtonInternal();

private:
	UPROPERTY()
	TArray<UItemSlotWidgetBase*> ItemSlotWidgetList;

	UPROPERTY()
	class UTextBlock* PriceText;

	UPROPERTY()
	class UButton* ChangeStoreBtn;

	UPROPERTY()
	class UEditableTextBox* StoreIDText;
};
