// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "../ItemDefine.h"
#include "StoreWidgetBase.generated.h"

/**
 * 
 */

class UItemSlotWidgetBase;
UCLASS()
class GAMEDATA_API UStoreWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()
	enum {MaxStoreSlotCount = 20};

private:
	virtual void NativeConstruct() final;
	virtual void OnDragDrop(class UDragDropOperation* InDragDropOp) final;
	virtual void OnButtonDown(class UButtonDownOperation* InButtonDownOp) final;
	
	bool TryBuyItem(int StoreSlotIndex);
	void UpdateStoreItemInfo(int InStoreClassID);

	void BindWidget();
	void UpdateWidgetSlotAll();

	UFUNCTION()
	void OnMouseOverSlot(int SlotIndex);

	UFUNCTION()
	void OnClickChangeStoreButton();

private:
	TArray<FItemInfo> StoreItemInfoList;

	UPROPERTY()
	TArray<UItemSlotWidgetBase*> ItemSlotWidgetList;

	UPROPERTY()
	class UTextBlock* PriceText;

	UPROPERTY()
	class UButton* ChangeStoreBtn;

	UPROPERTY()
	class UEditableTextBox* StoreIDText;
};
