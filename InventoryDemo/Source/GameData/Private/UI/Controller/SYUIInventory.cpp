// Fill out your copyright notice in the Description page of Project Settings.


#include "SYUIInventory.h"
#include "StoreWidgetBase.h"
#include "SYUtil.h"
#include "SYDefine.h"
#include "SYCharacter.h"
#include "InventoryWidgetBase.h"

void USYUIInventory::Init()
{
	UINumber = EUINumber::Inventory;
	BindWidget();
	
	InitContainer();

	ASYCharacter* Character = SYUtil::GetCharacter(GetWorld());
	if (Character)
	{
		Character->OnUpdateCash().AddUFunction(this, FName("OnUpdateCash"));
	}

}

void USYUIInventory::BindWidget()
{
	InventoryWidget = SYUtil::GetWidget<UInventoryWidgetBase>(this, UINumber);
	if (InventoryWidget)
	{
		InventoryWidget->OnClickedTab().AddUFunction(this, FName("OnClickedTab"));
		InventoryWidget->OnSlotDragDrop().AddUFunction(this, FName("OnSlotDragDrop"));
		InventoryWidget->OnSlotRButtonDown().AddUFunction(this, FName("OnSlotRButtonDown"));
		InventoryWidget->OnClickeAddCash().AddUFunction(this, FName("OnClickedAddCash"));
	}
}


bool USYUIInventory::TryAddItem(FItemKey ItemKey, int ItemCount)
{
	if (!CanAddItem(ItemKey, ItemCount))
		return false;

	int TabIndex = ConvertItemTypeToTabIndex(ItemKey.Type);
	if (!IsStackableItem(ItemKey))
	{
		int SlotIndex = GetEmptySlotIndex(TabIndex);
		FInventoryItemInfo NewItemInfo = FItemInfoFactory::CreateInventoryItemInfo(GetWorld(), ItemKey, ItemCount, SlotIndex);
		AddItem(NewItemInfo);
	}
	else // IsStackable()
	{
		int RemainNewItemCount = ItemCount;
		TArray<const FInventoryItemInfo*> ItemInfoList = GetItemInfoList(ItemKey);
		for (int i = 0; i < ItemInfoList.Num(); ++i)
		{
			const FInventoryItemInfo& ItemInfo = *ItemInfoList[i];
			int AddableItemCount = ItemInfo.MaxStackCount - ItemInfo.Count;
			if (AddableItemCount != 0)
			{
				int AddItemCount = FMath::Clamp(RemainNewItemCount, 0, AddableItemCount);
				int ResultItemCount = AddItemCount + ItemInfo.Count;
				UpdateItemCount(TabIndex, ItemInfo.SlotIndex, ResultItemCount);

				RemainNewItemCount -= AddItemCount;
				if (RemainNewItemCount <= 0)
					break;
			}
		}

		if (RemainNewItemCount > 0)
		{
			int SlotIndex = GetEmptySlotIndex(TabIndex);
			FInventoryItemInfo NewItemInfo = FItemInfoFactory::CreateInventoryItemInfo(GetWorld(), ItemKey, RemainNewItemCount, SlotIndex);
			AddItem(NewItemInfo);
		}
	}

	return true;
}

bool USYUIInventory::TrySubtractItem(int TabIndex, int SubtractCount, int SlotIndex)
{
	const FInventoryItemInfo& ItemInfo = GetItemInfo(TabIndex, SlotIndex);
	if (ItemInfo.ItemKey.ID == 0)
		return false;

	if (ItemInfo.Count < SubtractCount)
		return false;

	if (SubtractCount == ItemInfo.Count)
	{
		RemoveItem(TabIndex, SlotIndex);
	}
	else
	{
		int RemainItemCount = ItemInfo.Count - SubtractCount;
		UpdateItemCount(TabIndex, SlotIndex, RemainItemCount);
	}

	return true;
}

bool USYUIInventory::TrySubtractItemInSlotOrder(const FItemInfo& SubtractItemInfo, int SubtractCount)
{
	int ItemCount = GetItemCount(SubtractItemInfo.ItemKey);
	if (ItemCount < SubtractCount)
		return false;

	int TabIndex = ConvertItemTypeToTabIndex(SubtractItemInfo.ItemKey.Type);

	int RemainSubtractCount = SubtractCount;
	TArray<const FInventoryItemInfo*> ItemInfoList = GetItemInfoList(SubtractItemInfo.ItemKey);
	for (int i = 0; i < ItemInfoList.Num(); ++i)
	{
		const FInventoryItemInfo& ItemInfo = *(ItemInfoList[i]);
		if (RemainSubtractCount >= ItemInfo.Count)
		{
			RemainSubtractCount -= ItemInfo.Count;
			RemoveItem(TabIndex, ItemInfo.SlotIndex);

			if (RemainSubtractCount == 0)
				break;
		}
		else if (RemainSubtractCount < ItemInfo.Count)
		{
			int RemainCount = ItemInfo.Count - RemainSubtractCount;
			UpdateItemCount(TabIndex, ItemInfo.SlotIndex, RemainCount);
			break;
		}
	}

	return true;
}

void USYUIInventory::RemoveItem(int TabIndex, int SlotIndex)
{
	RemoveItemInfo(TabIndex, SlotIndex);

	if (CurrentTabIndex == TabIndex)
	{
		if (InventoryWidget)
			InventoryWidget->UpdateSlot(SlotIndex, GetItemInfo(TabIndex, SlotIndex));
	}
}


////////////
void USYUIInventory::OnSlotDragDrop(EUINumber SrcUINumber, int32 SrcSlotIndex, EUINumber DstUINumber, int32 DstSlotIndex)
{
	if (DstUINumber == EUINumber::Inventory)
	{
		if (SrcSlotIndex == DstSlotIndex)
			return;
	
		const FInventoryItemInfo& SrcItemInfo = GetItemInfo(CurrentTabIndex, SrcSlotIndex);
		const FInventoryItemInfo& DstItemInfo = GetItemInfo(CurrentTabIndex, DstSlotIndex);
		bool IsEmptySrcSlot = SrcItemInfo.ItemKey.ID == 0;
		bool IsEmptyDstSlot = DstItemInfo.ItemKey.ID == 0;
		if (!IsEmptySrcSlot)
		{
			if (IsEmptyDstSlot)
			{
				FInventoryItemInfo NewItemInfo = FItemInfoFactory::CreateInventoryItemInfo(GetWorld(), SrcItemInfo.ItemKey, SrcItemInfo.Count, DstSlotIndex);
				AddItem(NewItemInfo);
				RemoveItem(CurrentTabIndex, SrcSlotIndex);
			}
			else // !IsEmptyDstSlot
			{
				SwapItem(SrcSlotIndex, DstSlotIndex);
			}
		}
	}
}

void USYUIInventory::OnSlotRButtonDown(int SlotIndex)
{
	UWidgetManager* WidgetManager = SYUtil::GetWidgetManager(GetWorld());
	if (WidgetManager && WidgetManager->IsVisible(EUINumber::Store))
	{
		ASYCharacter* Character = SYUtil::GetCharacter(GetWorld());
		if (!Character)
			return;

		const FInventoryItemInfo& ItemInfo = GetItemInfo(CurrentTabIndex, SlotIndex);
		Character->TrySellItem(CurrentTabIndex, ItemInfo.Count, ItemInfo.Price, ItemInfo.SlotIndex);
	}
	else
	{
		//test
		TrySubtractItem(CurrentTabIndex, 1, SlotIndex);
	}
}

void USYUIInventory::OnClickedTab(int TabIndex)
{
	int PrevTabIndex = CurrentTabIndex;
	CurrentTabIndex = TabIndex;

	if (InventoryWidget)
	{
		InventoryWidget->UpdateTabColor(PrevTabIndex, CurrentTabIndex);

		for (int SlotIndex = 0; SlotIndex < UInventoryWidgetBase::InventorySlotCount; ++SlotIndex)
		{
			const FInventoryItemInfo& ItemInfo = GetItemInfo(CurrentTabIndex, SlotIndex);
			InventoryWidget->UpdateSlot(SlotIndex, ItemInfo);
		}
	}
}

void USYUIInventory::InitContainer()
{
	ItemInfoListMap.Empty();
	EmptySlotMap.Empty();

	const int InventorySlotCount = UInventoryWidgetBase::InventorySlotCount;

	// init ItemInfoListMap
	for (int TabIndex = 0; TabIndex < Tab_End; ++TabIndex)
	{
		ItemInfoListMap.Add(TabIndex, TArray<FInventoryItemInfo>());
		ItemInfoListMap[TabIndex].Reserve(InventorySlotCount);
		for (int InvenSlotIndex = 0; InvenSlotIndex < InventorySlotCount; ++InvenSlotIndex)
		{
			FInventoryItemInfo InvenItemInfo;
			InvenItemInfo.SlotIndex = InvenSlotIndex;
			ItemInfoListMap[TabIndex].Add(InvenItemInfo);
		}
	}

	// init EmptySlotMap
	for (int TabIndex = 0; TabIndex < Tab_End; ++TabIndex)
	{
		EmptySlotMap.Add(TabIndex, TArray<int>());
		EmptySlotMap[TabIndex].Reserve(InventorySlotCount);
		for (int invenSlotIndex = 0; invenSlotIndex < InventorySlotCount; ++invenSlotIndex)
		{
			EmptySlotMap[TabIndex].Add(invenSlotIndex);
		}
	}
}

const FInventoryItemInfo& USYUIInventory::GetItemInfo(int TabIndex, int SlotIndex)
{
	TArray<FInventoryItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (ItemInfoList && ItemInfoList->IsValidIndex(SlotIndex))
	{
		return (*ItemInfoList)[SlotIndex];
	}
	else
	{
		return FInventoryItemInfo::Empty;
	}
}

int USYUIInventory::GetEmptySlotIndex(int TabIndex)
{
	int EmptySlotIndex = INDEX_NONE;
	ArrayHeap* EmptySlotHeap = EmptySlotMap.Find(TabIndex);
	if (EmptySlotHeap && EmptySlotHeap->Num() > 0)
	{
		EmptySlotIndex = EmptySlotHeap->HeapTop();
	}

	return EmptySlotIndex;
}

int USYUIInventory::GetEmptySlotCount(int TabIndex)
{
	int EmptySlotCount = 0;
	ArrayHeap* EmptySlotHeap = EmptySlotMap.Find(TabIndex);
	if (EmptySlotHeap)
	{
		EmptySlotCount = EmptySlotHeap->Num();
	}

	return EmptySlotCount;
}

int USYUIInventory::GetItemCount(FItemKey ItemKey)
{
	int OutItemCount = 0;
	TArray<const FInventoryItemInfo*> ItemInfoList = GetItemInfoList(ItemKey);
	for (const FInventoryItemInfo* ItemInfo : ItemInfoList)
	{
		OutItemCount += ItemInfo->Count;
	}
	return OutItemCount;
}

void USYUIInventory::UpdateItemCount(int TabIndex, int SlotIndex, int ItemCount)
{
	TArray<FInventoryItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);

	if (ItemInfoList && ItemInfoList->IsValidIndex(SlotIndex))
	{
		FInventoryItemInfo& InventoryItemInfo = (*ItemInfoList)[SlotIndex];
		InventoryItemInfo.Count = ItemCount;

		if (CurrentTabIndex == TabIndex)
		{
			if (InventoryWidget)
				InventoryWidget->UpdateSlot(SlotIndex, InventoryItemInfo);
		}
	}

}

auto USYUIInventory::GetItemInfoList(FItemKey ItemKey) -> TArray<const FInventoryItemInfo*>
{
	TArray<const FInventoryItemInfo*> OutItemInfoList;

	int TabIndex = ConvertItemTypeToTabIndex(ItemKey.Type);
	TArray<FInventoryItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (ItemInfoList)
	{
		for (const FInventoryItemInfo& ItemInfo : *ItemInfoList)
		{
			if (ItemInfo.ItemKey.ID == ItemKey.ID)
				OutItemInfoList.Add(&ItemInfo);
		}
	}

	return OutItemInfoList;
}

void USYUIInventory::SetItemInfo(int TabIndex, const FInventoryItemInfo& NewItemInfo)
{
	TArray<FInventoryItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (ItemInfoList && ItemInfoList->IsValidIndex(NewItemInfo.SlotIndex))
	{
		(*ItemInfoList)[NewItemInfo.SlotIndex] = NewItemInfo;
	}
}

void USYUIInventory::UpdateItemInfo(const FInventoryItemInfo & InItemInfo)
{
	int TabIndex = ConvertItemTypeToTabIndex(InItemInfo.ItemKey.Type);
	const FInventoryItemInfo& PrevItemInfo = GetItemInfo(TabIndex, InItemInfo.SlotIndex);

	bool IsAddedItemToEmptySlot = PrevItemInfo.ItemKey.ID == 0 && InItemInfo.ItemKey.ID != 0;

	// set item info
	SetItemInfo(TabIndex, InItemInfo);

	// update empty slot (push)
	if (IsAddedItemToEmptySlot)
	{
		ArrayHeap* EmptySlotIndexHeap = EmptySlotMap.Find(TabIndex);
		if (EmptySlotIndexHeap)
		{
			EmptySlotIndexHeap->Remove(InItemInfo.SlotIndex);
			EmptySlotIndexHeap->Heapify();
		}
	}
}

void USYUIInventory::RemoveItemInfo(int TabIndex, int SlotIndex)
{
	// remove item info
	FInventoryItemInfo EmptyItemInfo = FItemInfoFactory::CreateEmptyInventoryItemInfo(SlotIndex);
	SetItemInfo(TabIndex, EmptyItemInfo);

	// update empty slot (pop)
	ArrayHeap* EmptySlotIndexHeap = EmptySlotMap.Find(TabIndex);
	if (EmptySlotIndexHeap)
	{
		EmptySlotIndexHeap->AddUnique(SlotIndex);
		EmptySlotIndexHeap->Heapify();
	}
}

bool USYUIInventory::IsStackableItem(FItemKey ItemKey)
{
	FSYTableItemBase* ItemTable = SYUtil::GetGameData<FSYTableItemBase>(GetWorld(), ItemKey.GetTableType(), ItemKey.ID);
	if (ItemTable && ItemTable->MaxStackCount > 1)
		return true;

	return false;
}

bool USYUIInventory::CanAddItem(FItemKey ItemKey, int ItemCount)
{
	int TabIndex = ConvertItemTypeToTabIndex(ItemKey.Type);
	int EmptySlotCount = GetEmptySlotCount(TabIndex);

	if (EmptySlotCount > 0)
		return true;

	if (IsStackableItem(ItemKey))
	{
		int AddableItemCount = 0;
		TArray<const FInventoryItemInfo*> ItemInfoList = GetItemInfoList(ItemKey);
		for (int i = 0; i < ItemInfoList.Num(); ++i)
		{
			AddableItemCount += ItemInfoList[i]->MaxStackCount - ItemInfoList[i]->Count;
			if (AddableItemCount >= ItemCount)
				return true;
		}
	}

	return false;
}

void USYUIInventory::AddItem(const FInventoryItemInfo& AddedItemInfo)
{
	UpdateItemInfo(AddedItemInfo);

	int TabIndex = ConvertItemTypeToTabIndex(AddedItemInfo.ItemKey.Type);
	if (CurrentTabIndex == TabIndex)
	{
		if (InventoryWidget)
		{
			InventoryWidget->UpdateSlot(AddedItemInfo.SlotIndex, AddedItemInfo);
		}
	}
}

void USYUIInventory::SwapItem(int SrcSlotIndex, int DstSlotIndex)
{
	FInventoryItemInfo SrcItemInfo = GetItemInfo(CurrentTabIndex, SrcSlotIndex);
	SrcItemInfo.SlotIndex = DstSlotIndex;

	FInventoryItemInfo DstItemInfo = GetItemInfo(CurrentTabIndex, DstSlotIndex);
	DstItemInfo.SlotIndex = SrcSlotIndex;

	UpdateItemInfo(DstItemInfo);
	UpdateItemInfo(SrcItemInfo);

	if (InventoryWidget)
	{
		InventoryWidget->UpdateSlot(DstItemInfo.SlotIndex, DstItemInfo);
		InventoryWidget->UpdateSlot(SrcItemInfo.SlotIndex, SrcItemInfo);
	}
}

int USYUIInventory::ConvertItemTypeToTabIndex(EItemType ItemType)
{
	if (ItemType == EItemType::Equipment)
	{
		return EquipmentTab;
	}
	else if (ItemType == EItemType::Consumable)
	{
		return ConsumableTab;
	}
	else if (ItemType == EItemType::Etc)
	{
		return EtcTab;
	}

	return Tab_End;
}

void USYUIInventory::OnClickedAddCash()
{
	ASYCharacter* Character = SYUtil::GetCharacter(GetWorld());
	if (Character)
	{
		
		Character->UpdateCash(Character->GetCash() + 2000);
	}
}

void USYUIInventory::OnUpdateCash(int Cash)
{
	if(InventoryWidget)
		InventoryWidget->UpdateCash(Cash);
}
