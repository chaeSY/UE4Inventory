// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYUIBase.h"
#include "ItemInfo.h"
#include "SYUIStore.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATA_API USYUIStore : public USYUIBase
{
	GENERATED_BODY()

public:
	void Bind(); // temp

private:
	bool TryBuyItem(int StoreSlotIndex);
	void InitStoreItemInfo(int InStoreClassID);

	UFUNCTION()
	void OnDragDrop(class UDragDropOperation* DragDropOp);

	UFUNCTION()
	void OnMouseButtonDown(class USYMouseButtonDownOp* MouseButtonDownOp);

	UFUNCTION()
	void OnMouseOverInSlot(int SlotIndex);

	UFUNCTION()
	void OnClickedChangeStoreButton(int StoreID);

private:
	class UStoreWidgetBase* StoreWidget;
	TArray<FStoreItemInfo>  StoreItemInfoList;
};
