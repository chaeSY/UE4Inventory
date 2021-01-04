// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYUIBase.h"
#include "ItemInfo.h"
#include "SYUIInventory.generated.h"

/**
 * 
 */
class UInventoryWidgetBase;
class USYWidgetBase;

UCLASS()
class GAMEDATA_API USYUIInventory : public USYUIBase
{
	GENERATED_BODY()

public:
	virtual void Init(EUINumber InUINumber, USYWidgetBase* InWidget) override final;

	bool TryAddItem(FItemKey ItemKey, int ItemCount);
	bool TrySubtractItem(int TabIndex, int SubtractCount, int SlotIndex);
	bool TrySubtractItemInSlotOrder(const FItemInfo& SubtractItemInfo, int SubtractCount);
	void RemoveItem(int TabIndex, int SlotIndex);

private:
	void InitContainer();
	void BindWidget();

	auto GetItemInfo(int TabIndex, int SlotIndex) ->const FInventoryItemInfo&;
	int	 GetEmptySlotIndex(int TabIndex);
	int	 GetEmptySlotCount(int TabIndex);

	int  GetItemCount(FItemKey ItemKey);
	void UpdateItemCount(int TabIndex, int SlotIndex, int ItemCount);

	auto GetItemInfoList(FItemKey ItemKey)->TArray<const FInventoryItemInfo*>;
	void SetItemInfo(int TabIndex, const FInventoryItemInfo& NewItemInfo);
	void UpdateItemInfo(const FInventoryItemInfo& ItemInfo);
	void RemoveItemInfo(int TabIndex, int SlotIndex);

	bool IsStackableItem(FItemKey ItemKey);
	bool CanAddItem(FItemKey ItemKey, int ItemCount);
	void AddItem(const FInventoryItemInfo& AddedItemInfo);
	void SwapItem(int SrcSlotIndex, int DstSlotIndex);

	int ConvertItemTypeToTabIndex(EItemType ItemType);

	int CurrentTabIndex;
	using ArrayHeap = TArray<int>;
	TMap<int, TArray<FInventoryItemInfo>>	ItemInfoListMap;		// key: TabIndex Value: ItemInfoList
	TMap<int, ArrayHeap>					EmptySlotMap;			// Key: TabIndex Value: SlotIndexHeap

private:
	UInventoryWidgetBase* InventoryWidget;

private:
	UFUNCTION()
	void OnSlotDragDrop(EUINumber SrcUINumber, int32 SrcSlotIndex, EUINumber DstUINumber, int32 DstSlotIndex);

	UFUNCTION()
	void OnSlotRButtonDown(int SlotIndex);

	UFUNCTION()
	void OnClickedTab(int TabIndex);

	UFUNCTION()
	void OnClickedAddCash();

	UFUNCTION()
	void OnUpdateCash(int Cash);
};
