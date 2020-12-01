// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../ItemDefine.h"
#include "Containers/BinaryHeap.h"
#include "SYWidgetBase.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */

struct FInventoryItemInfo : public FItemInfo
{
	int SlotIndex = INDEX_NONE;
	static FInventoryItemInfo Create(const FItemInfo& ItemInfo, int SlotIndex);	
};

UCLASS(Blueprintable)
class GAMEDATA_API UInventoryWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()
	enum { InventorySlotCount = 8 };
	enum { EquipmentTab = 0, ConsumableTab = 1, EtcTab = 2, Tab_End };

public:
	auto TryGetItemInfo(int TabIndex, int SlotIndex) -> const FInventoryItemInfo*;
	auto GetItemInfo(int tabIndex, int slotIndex)	 -> FInventoryItemInfo;

	bool TryAddItem(const FItemInfo& AddedItemInfo);
	bool TrySubtractItem(const FItemInfo& SubtractItemInfo, int SubtractCount);
	bool TrySubtractItem(int TabIndex, int SlotIndex, int SubtractCount);
	void RemoveItem(int TabIndex, int SlotIndex);

	void OnBuyItem();
	void OnClickTab(int TabIndex);

private:
	virtual void NativeConstruct() override;
	virtual void OnDragDrop(class UDragDropOperation* InDragDropOp);
	virtual void OnButtonDown(class UButtonDownOperation* InButtonDownOp);

	void BindWidget();
	void UpdateWidgetItemSlot(int SlotIndex);
	void UpdateWidgetItemSlotAll();
	void UpdateWidgetItemCount(int TabIndex, int SlotIndex, int ItemCount);
	void UpdateWidgetTabColor(int PrevTabIndex);
	void UpdateWidgetCash();

	void InitContainer();
	int	 GetEmptySlotIndex(int TabIndex);
	int	 GetEmptySlotCount(int TabIndex);

	int  GetItemCount(FItemKey ItemKey);
	void UpdateItemCount(int TabIndex, int SlotIndex, int ItemCount);

	auto GetItemInfoList(FItemKey ItemKey)->TArray<const FInventoryItemInfo*>;
	void SetItemInfo(int TabIndex, const FInventoryItemInfo& NewItemInfo);
	void UpdateItemInfo(const FInventoryItemInfo& ItemInfo);
	void RemoveItemInfo(int TabIndex, int SlotIndex);

	bool CanAddItem(const FItemInfo& NewItemInfo);
	void AddItem(const FItemInfo& AddedItemInfo, int SlotIndex);
	void SwapItem(int SrcSlotIndex, int DstSlotIndex);

	int ConvertItemTypeToTabIndex(EItemType ItemType);

private:
	TArray<class UItemSlotWidgetBase*> ItemSlotWidgetList;
	class UButton* TabButton[Tab_End];
	class UTextBlock* CashText;
	class UButton* AddCashButton;

	int CurrentTabIndex;
	using ArrayHeap = TArray<int>;
	TMap<int, TArray<FInventoryItemInfo>>	ItemInfoListMap;		// key: TabIndex Value: ItemInfoList
	TMap<int, ArrayHeap>					EmptySlotMap;			// Key: TabIndex Value: SlotIndexHeap

private:
	UFUNCTION()
	void OnClickEquipmentTab();
	
	UFUNCTION()
	void OnClickConsumableTab();
	
	UFUNCTION()
	void OnClickEtcTab();

	UFUNCTION()
	void OnClickAddCash();
};
