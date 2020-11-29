// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "ItemSlotWidgetBase.h"
#include "Operation/DragDropSlot.h"
#include "Operation/ButtonDownSlot.h"
#include "../GameData/SYGameDataManager.h"
#include "../GameData/SYDataTable.h"
#include "../SYPlayerController.h"
#include "../SYGameModeBase.h"
#include "../SYCharacter.h"
#include "../SYGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidgetBase::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);

	BindWidget();
	InitContainer();
	
	CurrentTabIndex = EquipmentTab;
	UpdateWidgetTabColor(INDEX_NONE);
	UpdateWidgetItemSlotAll();
	UpdateWidgetCash();
}

void UInventoryWidgetBase::OnClickTab(int TabIndex)
{
	int PrevTabIndex = CurrentTabIndex;
	CurrentTabIndex = TabIndex;

	UpdateWidgetTabColor(PrevTabIndex);
	UpdateWidgetItemSlotAll();
}

bool UInventoryWidgetBase::CanAddItem(const FItemInfo& NewItemInfo)
{
	int TabIndex = ConvertItemTypeToTabIndex(NewItemInfo.ItemKey.Type);
	int EmptySlotCount = GetEmptySlotCount(TabIndex);
	
	if (EmptySlotCount != 0)
		return true;

	if (NewItemInfo.IsStackable())
	{
		TArray<int>* ItemPositionList = ItemPositionListMap.Find(NewItemInfo.ItemKey);
		if (ItemPositionList)
		{
			int AddableItemCount = 0;
			for (int i = 0; i < ItemPositionList->Num(); ++i)
			{
				const FItemInfo& ItemInfo = GetItemInfo(TabIndex, (*ItemPositionList)[i]);
				AddableItemCount += ItemInfo.MaxStackCount - ItemInfo.Count;
			
				if (AddableItemCount >= NewItemInfo.Count)
					return true;
			}
		}
	}
	return false;
}

bool UInventoryWidgetBase::TryAddItem(FItemInfo NewItemInfo)
{
	if(!CanAddItem(NewItemInfo))
		return false;

	int TabIndex = ConvertItemTypeToTabIndex(NewItemInfo.ItemKey.Type);
	if (NewItemInfo.IsStackable())
	{
		int RemainNewItemCount = NewItemInfo.Count;
		TArray<int>* ItemPositionList = ItemPositionListMap.Find(NewItemInfo.ItemKey);
		if (ItemPositionList)
		{
			for (int i = 0; i < ItemPositionList->Num(); ++i)
			{
				int SlotIndex = (*ItemPositionList)[i];
				FItemInfo ItemInfo = GetItemInfo(TabIndex, SlotIndex);
				int AddableItemCount = ItemInfo.MaxStackCount - ItemInfo.Count;
				if (AddableItemCount != 0)
				{
					int AddItemCount = FMath::Clamp(RemainNewItemCount, 0, AddableItemCount);
					int ResultItemCount = AddItemCount + ItemInfo.Count;
					ItemInfo.Count = ResultItemCount;
					AddItem(ItemInfo, SlotIndex);
			
					RemainNewItemCount -= AddItemCount;
					if (RemainNewItemCount <= 0)
						break;
				}
			}
		}

		if (RemainNewItemCount > 0)
		{
			NewItemInfo.Count = RemainNewItemCount;
			AddItem(NewItemInfo);
		}
	}
	else // !IsStackable()
	{
		AddItem(NewItemInfo);
	}

	return true;
}

bool UInventoryWidgetBase::TrySubtractItem(const FItemInfo& SubtractItemInfo, int SubtractCount)
{
	FItemInfo* ItemInfo = ItemInfoMap.Find(SubtractItemInfo.ItemKey);
	if(!ItemInfo)
		return false;

	if(ItemInfo->Count < SubtractItemInfo.Count)
		return false;

	TArray<int>* ItemPositionList = ItemPositionListMap.Find(SubtractItemInfo.ItemKey);
	if (!ItemPositionList)
		return false;

	int TabIndex = ConvertItemTypeToTabIndex(SubtractItemInfo.ItemKey.Type);
	int RemainSubtractCount = SubtractCount;
	for (int i = 0; i < ItemPositionList->Num(); ++i)
	{
		int SlotIndex = (*ItemPositionList)[i];
		FItemInfo ItemInfoInSlot = GetItemInfo(TabIndex, SlotIndex);
		if (RemainSubtractCount >= ItemInfoInSlot.Count)
		{
			RemoveItem(TabIndex, SlotIndex);
			RemainSubtractCount -= ItemInfoInSlot.Count;
			if(RemainSubtractCount == 0)
				break;
		}
		else if (RemainSubtractCount < ItemInfoInSlot.Count)
		{
			SubtractItem(TabIndex, SlotIndex, RemainSubtractCount);
			break;
		}
	}

	return true;
}

bool UInventoryWidgetBase::TrySubtractItem(int TabIndex, int SlotIndex, int SubtractCount)
{
	TArray<FItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if(!ItemInfoList)
		return false;

	if(!ItemInfoList->IsValidIndex(SlotIndex))
		return false;

	const FItemInfo& SubtractItemInfo = (*ItemInfoList)[SlotIndex];
	if (SubtractCount >= SubtractItemInfo.Count)
	{
		RemoveItem(TabIndex, SlotIndex);
	}
	else 
	{
		SubtractItem(TabIndex, SlotIndex, SubtractCount);
	}

	return true;
}

void UInventoryWidgetBase::RemoveItem(int TabIndex, int SlotIndex)
{
	FItemInfo ItemInfo = GetItemInfo(TabIndex, SlotIndex);
	if (ItemInfo.ItemKey.ID == INVALID_CLASSID)
		return;

	SetItemInfo(TabIndex, SlotIndex, FItemInfo());

	if (CurrentTabIndex == TabIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
	}
}

void UInventoryWidgetBase::AddItem(FItemInfo newItemInfo, int InSlotIndex /* = INDEX_NONE */)
{
	int TabIndex = ConvertItemTypeToTabIndex(newItemInfo.ItemKey.Type);
	int SlotIndex = InSlotIndex == INDEX_NONE ? GetEmptySlotIndex(TabIndex) : InSlotIndex;

	SetItemInfo(TabIndex, SlotIndex, newItemInfo);
	
	if (CurrentTabIndex == TabIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
	}
}

void UInventoryWidgetBase::SubtractItem(int TabIndex, int SlotIndex, int ItemCount)
{
	FItemInfo ItemInfo = GetItemInfo(TabIndex, SlotIndex);
	if (ItemInfo.ItemKey.ID == INVALID_CLASSID)
		return;
	
	ItemInfo.Count -= ItemCount;
	SetItemInfo(TabIndex, SlotIndex, ItemInfo);
	
	//update
	if (CurrentTabIndex == TabIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
	}
}

void UInventoryWidgetBase::OnBuyItem()
{
	UpdateWidgetCash();
}

void UInventoryWidgetBase::UpdateWidgetItemSlot(int SlotIndex)
{
	if(!ItemSlotWidgetList.IsValidIndex(SlotIndex))
		return;

	UItemSlotWidgetBase* SlotWidget = ItemSlotWidgetList[SlotIndex];
	if (!SlotWidget)
		return;

	FItemInfo ItemInfo = GetItemInfo(CurrentTabIndex, SlotIndex);
	SlotWidget->UpdateSlot(ItemInfo);
}

void UInventoryWidgetBase::UpdateWidgetItemSlotAll()
{
	for (int SlotIndex = 0; SlotIndex < InventorySlotCount; ++SlotIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
	}
}

void UInventoryWidgetBase::UpdateWidgetCash()
{
	if (!CashText)
		return;

	ASYPlayerController* SYController = Cast<ASYPlayerController>(GetOwningPlayer());
	if (!SYController)
		return;

	ASYCharacter* Character = Cast<ASYCharacter>(SYController->GetCharacter());
	if (!Character)
		return;

	int Cash = Character->GetCash();
	CashText->SetText(FText::AsNumber(Cash));
}

void UInventoryWidgetBase::InitContainer()
{
	ItemInfoMap.Empty();
	ItemPositionListMap.Empty();
	ItemInfoListMap.Empty();
	EmptySlotMap.Empty();

	// init ItemInfoListMap
	for (int TabIndex = 0; TabIndex < Tab_End; ++TabIndex)
	{
		ItemInfoListMap.Add(TabIndex, TArray<FItemInfo>());
		ItemInfoListMap[TabIndex].Reserve(InventorySlotCount);
		for (int invenSlotIndex = 0; invenSlotIndex < InventorySlotCount; ++invenSlotIndex)
		{
			ItemInfoListMap[TabIndex].Add(FItemInfo());
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

void UInventoryWidgetBase::BindWidget()
{
	// binding ItemSlotWidgetList
	for (int InvenSlotIdex = 0; InvenSlotIdex < InventorySlotCount; ++InvenSlotIdex)
	{
		FString SlotWidgetName = FString::Printf(TEXT("InventorySlot_%d"), InvenSlotIdex);
		UItemSlotWidgetBase* ItemSlotWidget = Cast<UItemSlotWidgetBase>(GetWidgetFromName(*SlotWidgetName));
		if (ItemSlotWidget)
		{
			ItemSlotWidget->SetSlotIndex(InvenSlotIdex);
			ItemSlotWidget->SetParentUINumber(EUINumber::Inventory);
			ItemSlotWidgetList.Add(ItemSlotWidget);
		}
	}

	// binding Tab
	for (int TabIndex = 0; TabIndex < Tab_End; ++TabIndex)
	{
		FString BorderName = FString::Printf(TEXT("TabButton_%d"), TabIndex);
		TabButton[TabIndex] = Cast<UButton>(GetWidgetFromName(*BorderName));
	}

	if (TabButton[EquipmentTab])
		TabButton[EquipmentTab]->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickEquipmentTab);

	if (TabButton[ConsumableTab])
		TabButton[ConsumableTab]->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickConsumableTab);

	if (TabButton[EtcTab])
		TabButton[EtcTab]->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickEtcTab);


	// binding cash
	CashText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CashText")));
	AddCashButton = Cast<UButton>(GetWidgetFromName(TEXT("AddCashButton")));
	if(AddCashButton)
		AddCashButton->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickAddCash);

}

FItemInfo UInventoryWidgetBase::GetItemInfo(int tabIndex, int slotIndex)
{
	TArray<FItemInfo>* itemInfoList = ItemInfoListMap.Find(tabIndex);
	if (itemInfoList && itemInfoList->IsValidIndex(slotIndex))
	{
		return (*itemInfoList)[slotIndex];
	}

	return FItemInfo();
}

void UInventoryWidgetBase::SetItemInfo(int TabIndex, int SlotIndex, FItemInfo InItemInfo) // update item data
{
	if(SlotIndex == INDEX_NONE)
		return;

	TArray<FItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (!ItemInfoList || !ItemInfoList->IsValidIndex(SlotIndex))
		return;
	
	FItemInfo PrevItemInfo = (*ItemInfoList)[SlotIndex];
	
	// update item info
	(*ItemInfoList)[SlotIndex] = InItemInfo;
	
	bool IsAddedItem = PrevItemInfo.ItemKey.ID == INVALID_CLASSID && InItemInfo.ItemKey.ID != INVALID_CLASSID;
	bool IsRemoveItem = PrevItemInfo.ItemKey.ID != INVALID_CLASSID && InItemInfo.ItemKey.ID == INVALID_CLASSID;
	if (IsAddedItem)
	{
		// add item position
		TArray<int>& ItemPositionList = ItemPositionListMap.FindOrAdd(InItemInfo.ItemKey);
		ItemPositionList.AddUnique(SlotIndex);

		// pop empty slot
		ArrayHeap* EmptySlotIndexHeap = EmptySlotMap.Find(TabIndex);
		if (EmptySlotIndexHeap)
		{
			EmptySlotIndexHeap->Remove(SlotIndex);
			EmptySlotIndexHeap->Heapify();
		}

		// add item info
		FItemInfo* ItemInfo = ItemInfoMap.Find(InItemInfo.ItemKey);
		if (ItemInfo)
		{
			ItemInfo->Count += InItemInfo.Count;
		}
		else
		{
			ItemInfoMap.Add(InItemInfo.ItemKey, InItemInfo);
		}
	}
	else if (IsRemoveItem)
	{
		// remove item position
		TArray<int>* ItemPositionList = ItemPositionListMap.Find(PrevItemInfo.ItemKey);
		if (ItemPositionList)
		{
			ItemPositionList->Remove(SlotIndex);
		}

		// push empty slot
		ArrayHeap* EmptySlotIndexHeap = EmptySlotMap.Find(TabIndex);
		if (EmptySlotIndexHeap)
		{
			EmptySlotIndexHeap->AddUnique(SlotIndex);
			EmptySlotIndexHeap->Heapify();
		}

		// subtract or remove ItemInfo
		FItemInfo* ItemInfo = ItemInfoMap.Find(PrevItemInfo.ItemKey);
		if (ItemInfo)
		{
			ItemInfo->Count -= InItemInfo.Count;
			if (ItemInfo->Count <= 0)
			{
				ItemInfoMap.Remove(InItemInfo.ItemKey);
			}
		}
	}
}

void UInventoryWidgetBase::UpdateWidgetItemCount(int TabIndex, int SlotIndex, int ItemCount) // update item count
{
	TArray<FItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (ItemInfoList && ItemInfoList->IsValidIndex(SlotIndex))
	{
		(*ItemInfoList)[SlotIndex].Count = ItemCount;
	}
}

void UInventoryWidgetBase::UpdateWidgetTabColor(int PrevTabIndex)
{
	if (PrevTabIndex >= 0 && PrevTabIndex < Tab_End)
	{
		FLinearColor DefaultTabColor(1.f, 1.f, 1.f, 0.8f);
		if (TabButton[PrevTabIndex])
		{
			TabButton[PrevTabIndex]->SetColorAndOpacity(DefaultTabColor);
			TabButton[PrevTabIndex]->SetBackgroundColor(DefaultTabColor);
		}
	}

	if (CurrentTabIndex >= 0 && CurrentTabIndex< Tab_End)
	{
		FLinearColor SelectedTabColor(1.f, 0.6f, 0.f, 1.f);
		if (TabButton[CurrentTabIndex])
		{
			TabButton[CurrentTabIndex]->SetColorAndOpacity(SelectedTabColor);
			TabButton[CurrentTabIndex]->SetBackgroundColor(SelectedTabColor);
		}
	}
}

int UInventoryWidgetBase::GetEmptySlotIndex(int TabIndex)
{
	int EmptySlotIndex = INDEX_NONE;
	ArrayHeap* EmptySlotHeap = EmptySlotMap.Find(TabIndex);
	if (EmptySlotHeap && EmptySlotHeap->Num() > 0)
	{
		EmptySlotIndex = EmptySlotHeap->HeapTop();
	}

	return EmptySlotIndex;
}

int UInventoryWidgetBase::GetEmptySlotCount(int TabIndex)
{
	int EmptySlotCount = 0;
	ArrayHeap* EmptySlotHeap = EmptySlotMap.Find(TabIndex);
	if (EmptySlotHeap)
	{
		EmptySlotCount = EmptySlotHeap->Num();
	}

	return EmptySlotCount;
}


int UInventoryWidgetBase::ConvertItemTypeToTabIndex(EItemType ItemType)
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

void UInventoryWidgetBase::OnDragDrop(UDragDropOperation* InDragDropOp)
{
	if (InDragDropOp->IsA(UDragDropSlot::StaticClass()))
	{
		UDragDropSlot* DragDrop = Cast<UDragDropSlot>(InDragDropOp);
		if (DragDrop->ToUINumber == EUINumber::Inventory)
		{
			if (DragDrop->FromSlotIndex == DragDrop->ToSlotIndex)
			{
				return;
			}
			
			FItemInfo SrcItemInfo = GetItemInfo(CurrentTabIndex, DragDrop->FromSlotIndex);
			FItemInfo DstItemInfo = GetItemInfo(CurrentTabIndex, DragDrop->ToSlotIndex);
			bool IsEmptySrcSlot = SrcItemInfo.ItemKey.ID == 0;
			bool IsEmptyDstSlot = DstItemInfo.ItemKey.ID == 0;
			if (!IsEmptySrcSlot)
			{
				if (IsEmptyDstSlot)
				{
					RemoveItem(CurrentTabIndex, DragDrop->FromSlotIndex);
					AddItem(SrcItemInfo, DragDrop->ToSlotIndex);
				}
				else // !IsEmptyDstSlot
				{
					SwapItem(DragDrop->FromSlotIndex, DragDrop->ToSlotIndex);
				}
			}
		}
	}
}

void UInventoryWidgetBase::OnButtonDown(class UButtonDownOperation* InButtonDownOp)
{
	if (InButtonDownOp->IsA(UButtonDownSlot::StaticClass()))
	{
		UButtonDownSlot* ButtonDownOp = Cast<UButtonDownSlot>(InButtonDownOp);
		if (ButtonDownOp->PressedKey == EKeys::RightMouseButton)
		{
			TrySubtractItem(CurrentTabIndex, ButtonDownOp->SlotIndex, 1);
		}
	}
}

void UInventoryWidgetBase::SwapItem(int LeftSlotIndex, int RightSlotIndex)
{
	FItemInfo LeftItemInfo  = GetItemInfo(CurrentTabIndex, LeftSlotIndex);
	FItemInfo RightItemInfo = GetItemInfo(CurrentTabIndex, RightSlotIndex);
	
	SetItemInfo(CurrentTabIndex, LeftSlotIndex, FItemInfo());
	SetItemInfo(CurrentTabIndex, RightSlotIndex, FItemInfo());

	SetItemInfo(CurrentTabIndex, LeftSlotIndex, RightItemInfo);
	SetItemInfo(CurrentTabIndex, RightSlotIndex, LeftItemInfo);

	UpdateWidgetItemSlot(LeftSlotIndex);
	UpdateWidgetItemSlot(RightSlotIndex);
}

void UInventoryWidgetBase::OnClickEquipmentTab()
{
	OnClickTab(EquipmentTab);
}

void UInventoryWidgetBase::OnClickConsumableTab()
{
	OnClickTab(ConsumableTab);
}

void UInventoryWidgetBase::OnClickEtcTab()
{
	OnClickTab(EtcTab);
}

void UInventoryWidgetBase::OnClickAddCash()
{
	ASYPlayerController* SYController = Cast<ASYPlayerController>(GetOwningPlayer());
	if (!SYController)
		return;

	ASYCharacter* Character = Cast<ASYCharacter>(SYController->GetCharacter());
	if(!Character)
		return;

	Character->AddCash(500);
	UpdateWidgetCash();
}
