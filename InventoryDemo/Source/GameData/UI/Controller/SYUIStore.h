// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYUIBase.h"
#include "ItemInfo.h"
#include "SYUIStore.generated.h"

/**
 * 
 */
class UStoreWidgetBase;
class USYWidgetBase;

UCLASS()
class GAMEDATA_API USYUIStore : public USYUIBase
{
	GENERATED_BODY()

public:
	virtual void Init() final;

private:
	void BindWidget();

	bool TryBuyItem(int StoreSlotIndex);
	void InitStoreItemInfo(int InStoreClassID);

	UFUNCTION()
	void OnDragDropSlot(EUINumber SrcUINumber, int32 StoreSlotIndex, EUINumber DstUINumber, int32 DstSlotIndex);

	UFUNCTION()
	void OnMouseRButtonDownSlot(int SlotIndex);

	UFUNCTION()
	void OnMouseOverInSlot(int SlotIndex);

	UFUNCTION()
	void OnClickedChangeStoreButton(int StoreID);

private:
	UPROPERTY()
	UStoreWidgetBase* StoreWidget;

	TArray<FStoreItemInfo>  StoreItemInfoList;
};
