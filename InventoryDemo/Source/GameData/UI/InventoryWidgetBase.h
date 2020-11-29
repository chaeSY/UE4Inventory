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
UCLASS(Blueprintable)
class GAMEDATA_API UInventoryWidgetBase : public USYWidgetBase
{
	GENERATED_BODY()
	enum { InventorySlotCount = 8 };
	enum { EquipmentTab = 0, ConsumableTab = 1, EtcTab = 2, Tab_End };

public:
	FItemInfo GetItemInfo(int tabIndex, int slotIndex);
	void	  SetItemInfo(int TabIndex, int SlotIndex, FItemInfo ItemInfo);

	bool TryAddItem(FItemInfo NewItemInfo);
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
	bool CanAddItem(const FItemInfo& NewItemInfo);
	void AddItem(FItemInfo newItemInfo, int SlotIndex = INDEX_NONE);
	void SubtractItem(int TabIndex, int SlotIndex, int ItemCount);
	void SwapItem(int LeftSlotIndex, int RightSlotIndex);
	
	int ConvertItemTypeToTabIndex(EItemType ItemType);

private:
	//widget
	TArray<class UItemSlotWidgetBase*> ItemSlotWidgetList;
	class UButton* TabButton[Tab_End];
	class UTextBlock* CashText;
	class UButton* AddCashButton;

	//container
	int CurrentTabIndex;
	TMap<int, TArray<FItemInfo>>	 ItemInfoListMap;		// key: tabIndex Value: ItemInfoList
	TMap<FItemKey, FItemInfo>		 ItemInfoMap;			// Key: ItemKey	 Value: ItemInfo
	TMap<FItemKey, TArray<int>>		 ItemPositionListMap;	// Key: ItemKey	 Value: SlotIndexList
	using ArrayHeap = TArray<int>;
	TMap<int, ArrayHeap>			 EmptySlotMap;			// Key: tabIndex Value: SlotIndexHeap

private:
	//HACK
	UFUNCTION()
	void OnClickEquipmentTab();
	
	UFUNCTION()
	void OnClickConsumableTab();
	
	UFUNCTION()
	void OnClickEtcTab();

	UFUNCTION()
	void OnClickAddCash();
};
