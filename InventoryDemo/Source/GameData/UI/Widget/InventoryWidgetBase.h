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
UCLASS(Blueprintable)
class GAMEDATA_API UInventoryWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()
	enum { InventorySlotCount = 8 };
	enum { EquipmentTab = 0, ConsumableTab = 1, EtcTab = 2, Tab_End };

public:
	bool TryAddItem(FItemKey ItemKey, int ItemCount);
	bool TrySubtractItem(int TabIndex, int SubtractCount, int SlotIndex);
	bool TrySubtractItemInSlotOrder(const FItemInfo& SubtractItemInfo, int SubtractCount);
	void RemoveItem(int TabIndex, int SlotIndex);

	void OnBuyItem();
	void OnClickTab(int TabIndex);

private:
	virtual void NativeConstruct() override;
	virtual void OnDragDrop(class UDragDropOperation* InDragDropOp);
	virtual void OnButtonDown(class USYMouseButtonDownOp* InButtonDownOp);

	void BindWidget();
	void UpdateWidgetItemSlot(int SlotIndex);
	void UpdateWidgetItemSlotAll();
	void UpdateWidgetTabColor(int PrevTabIndex);
	void UpdateWidgetCash();

	TArray<class UItemSlotWidgetBase*> ItemSlotWidgetList;
	class UButton* TabButton[Tab_End];
	class UTextBlock* CashText;
	class UButton* AddCashButton;


private:
	void InitContainer();
	
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
	UFUNCTION()
	void OnClickEquipmentTab();
	
	UFUNCTION()
	void OnClickConsumableTab();
	
	UFUNCTION()
	void OnClickEtcTab();

	UFUNCTION()
	void OnClickAddCash();
};
