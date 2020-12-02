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
#include "HAL/UnrealMemory.h"

FInventoryItemInfo FInventoryItemInfo::Create(const FItemInfo& ItemInfo, int SlotIndex)
{
	FInventoryItemInfo InventoryItemInfo;
	FMemory::Memcpy(&InventoryItemInfo, &ItemInfo, sizeof(FItemInfo));
	InventoryItemInfo.SlotIndex = SlotIndex;

	return InventoryItemInfo;
}

auto UInventoryWidgetBase::TryGetItemInfo(int TabIndex, int SlotIndex) -> const FInventoryItemInfo*
{
	TArray<FInventoryItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (ItemInfoList && ItemInfoList->IsValidIndex(SlotIndex))
	{
		return &(*ItemInfoList)[SlotIndex];
	}

	return nullptr;
}

auto UInventoryWidgetBase::GetItemInfo(int TabIndex, int SlotIndex) -> FInventoryItemInfo
{
	TArray<FInventoryItemInfo>* itemInfoList = ItemInfoListMap.Find(TabIndex);
	if (itemInfoList && itemInfoList->IsValidIndex(SlotIndex))
	{
		return (*itemInfoList)[SlotIndex];
	}

	return FInventoryItemInfo();
}

bool UInventoryWidgetBase::TryAddItem(const FItemInfo& AddedItemInfo)
{
	if (!CanAddItem(AddedItemInfo))
		return false;

	int TabIndex = ConvertItemTypeToTabIndex(AddedItemInfo.ItemKey.Type);
	if (!AddedItemInfo.IsStackable())
	{
		int SlotIndex = GetEmptySlotIndex(TabIndex);
		AddItem(AddedItemInfo, SlotIndex);
	}
	else // IsStackable()
	{
		int RemainNewItemCount = AddedItemInfo.Count;
		TArray<const FInventoryItemInfo*> ItemInfoList = GetItemInfoList(AddedItemInfo.ItemKey);
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
			FItemInfo NewItemInfo = AddedItemInfo;
			NewItemInfo.Count = RemainNewItemCount;

			int SlotIndex = GetEmptySlotIndex(TabIndex);
			AddItem(NewItemInfo, SlotIndex);
		}
	}

	return true;
}

bool UInventoryWidgetBase::TrySubtractItem(const FItemInfo& SubtractItemInfo, int SubtractCount)
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

bool UInventoryWidgetBase::TrySubtractItem(int TabIndex, int SlotIndex, int SubtractCount)
{
	const FItemInfo* ItemInfo = TryGetItemInfo(TabIndex, SlotIndex);
	if (!ItemInfo)
		return false;

	if (ItemInfo->Count < SubtractCount)
		return false;

	if (SubtractCount == ItemInfo->Count)
	{
		RemoveItem(TabIndex, SlotIndex);
	}
	else
	{
		int RemainItemCount = ItemInfo->Count - SubtractCount;
		UpdateItemCount(TabIndex, SlotIndex, RemainItemCount);
	}

	return true;
}

void UInventoryWidgetBase::RemoveItem(int TabIndex, int SlotIndex)
{
	RemoveItemInfo(TabIndex, SlotIndex);

	if (CurrentTabIndex == TabIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
	}
}

void UInventoryWidgetBase::OnBuyItem()
{
	UpdateWidgetCash();
}

void UInventoryWidgetBase::OnClickTab(int TabIndex)
{
	int PrevTabIndex = CurrentTabIndex;
	CurrentTabIndex = TabIndex;

	UpdateWidgetTabColor(PrevTabIndex);
	UpdateWidgetItemSlotAll();
}

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

			const FInventoryItemInfo* SrcItemInfo = TryGetItemInfo(CurrentTabIndex, DragDrop->FromSlotIndex);
			const FInventoryItemInfo* DstItemInfo = TryGetItemInfo(CurrentTabIndex, DragDrop->ToSlotIndex);
			if (SrcItemInfo && DstItemInfo)
			{
				bool IsEmptySrcSlot = SrcItemInfo->ItemKey.ID == 0;
				bool IsEmptyDstSlot = DstItemInfo->ItemKey.ID == 0;
				if (!IsEmptySrcSlot)
				{
					if (IsEmptyDstSlot)
					{
						AddItem(*SrcItemInfo, DragDrop->ToSlotIndex);
						RemoveItem(CurrentTabIndex, DragDrop->FromSlotIndex);
					}
					else // !IsEmptyDstSlot
					{
						SwapItem(DragDrop->FromSlotIndex, DragDrop->ToSlotIndex);
					}
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
	if (AddCashButton)
		AddCashButton->OnClicked.AddDynamic(this, &UInventoryWidgetBase::OnClickAddCash);

}

void UInventoryWidgetBase::UpdateWidgetItemSlot(int SlotIndex)
{
	if (!ItemSlotWidgetList.IsValidIndex(SlotIndex))
		return;

	UItemSlotWidgetBase* SlotWidget = ItemSlotWidgetList[SlotIndex];
	if (!SlotWidget)
		return;

	const FItemInfo* ItemInfo = TryGetItemInfo(CurrentTabIndex, SlotIndex);
	if (ItemInfo)
	{
		SlotWidget->UpdateSlot(*ItemInfo);
	}
}

void UInventoryWidgetBase::UpdateWidgetItemSlotAll()
{
	for (int SlotIndex = 0; SlotIndex < InventorySlotCount; ++SlotIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
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

	if (CurrentTabIndex >= 0 && CurrentTabIndex < Tab_End)
	{
		FLinearColor SelectedTabColor(1.f, 0.6f, 0.f, 1.f);
		if (TabButton[CurrentTabIndex])
		{
			TabButton[CurrentTabIndex]->SetColorAndOpacity(SelectedTabColor);
			TabButton[CurrentTabIndex]->SetBackgroundColor(SelectedTabColor);
		}
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
	ItemInfoListMap.Empty();
	EmptySlotMap.Empty();

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

int UInventoryWidgetBase::GetItemCount(FItemKey ItemKey)
{
	int OutItemCount = 0;
	TArray<const FInventoryItemInfo*> ItemInfoList = GetItemInfoList(ItemKey);
	for (const FInventoryItemInfo* ItemInfo : ItemInfoList)
	{
		OutItemCount += ItemInfo->Count;
	}
	return OutItemCount;
}

void UInventoryWidgetBase::UpdateItemCount(int TabIndex, int SlotIndex, int ItemCount)
{
	TArray<FInventoryItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (ItemInfoList && ItemInfoList->IsValidIndex(SlotIndex))
	{
		(*ItemInfoList)[SlotIndex].Count = ItemCount;
	}

	if (CurrentTabIndex == TabIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
	}
}

auto UInventoryWidgetBase::GetItemInfoList(FItemKey ItemKey) -> TArray<const FInventoryItemInfo*>
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

void UInventoryWidgetBase::SetItemInfo(int TabIndex, const FInventoryItemInfo& NewItemInfo)
{
	TArray<FInventoryItemInfo>* ItemInfoList = ItemInfoListMap.Find(TabIndex);
	if (ItemInfoList && ItemInfoList->IsValidIndex(NewItemInfo.SlotIndex))
	{
		(*ItemInfoList)[NewItemInfo.SlotIndex] = NewItemInfo;
	}
}

void UInventoryWidgetBase::UpdateItemInfo(const FInventoryItemInfo & InItemInfo)
{
	int TabIndex = ConvertItemTypeToTabIndex(InItemInfo.ItemKey.Type);
	const FInventoryItemInfo* PrevItemInfo = TryGetItemInfo(TabIndex, InItemInfo.SlotIndex);
	if (!PrevItemInfo)
		return;

	bool IsAddedItem = PrevItemInfo->ItemKey.ID == INVALID_CLASSID && InItemInfo.ItemKey.ID != INVALID_CLASSID;
	bool IsChangedItem = PrevItemInfo->ItemKey.ID != INVALID_CLASSID && InItemInfo.ItemKey.ID != INVALID_CLASSID;

	// update item info
	SetItemInfo(TabIndex, InItemInfo);

	//update container
	if (IsAddedItem)
	{
		// pop empty slot
		ArrayHeap* EmptySlotIndexHeap = EmptySlotMap.Find(TabIndex);
		if (EmptySlotIndexHeap)
		{
			EmptySlotIndexHeap->Remove(InItemInfo.SlotIndex);
			EmptySlotIndexHeap->Heapify();
		}
	}
}

void UInventoryWidgetBase::RemoveItemInfo(int TabIndex, int SlotIndex)
{
	// remove item info
	FInventoryItemInfo NewItemInfo = FInventoryItemInfo::Create(FItemInfo(), SlotIndex);
	SetItemInfo(TabIndex, NewItemInfo);

	// push empty slot
	ArrayHeap* EmptySlotIndexHeap = EmptySlotMap.Find(TabIndex);
	if (EmptySlotIndexHeap)
	{
		EmptySlotIndexHeap->AddUnique(SlotIndex);
		EmptySlotIndexHeap->Heapify();
	}
}

bool UInventoryWidgetBase::CanAddItem(const FItemInfo& AddedItemInfo)
{
	int TabIndex = ConvertItemTypeToTabIndex(AddedItemInfo.ItemKey.Type);
	int EmptySlotCount = GetEmptySlotCount(TabIndex);
	
	if (EmptySlotCount > 0)
		return true;

	if (AddedItemInfo.IsStackable())
	{
		int AddableItemCount = 0;
		TArray<const FInventoryItemInfo*> ItemInfoList = GetItemInfoList(AddedItemInfo.ItemKey);
		for (int i = 0; i < ItemInfoList.Num(); ++i)
		{
			AddableItemCount += ItemInfoList[i]->MaxStackCount - ItemInfoList[i]->Count;
			if (AddableItemCount >= AddedItemInfo.Count)
				return true;
		}
	}
	return false;
}

void UInventoryWidgetBase::AddItem(const FItemInfo& AddedItemInfo, int SlotIndex)
{
	int TabIndex = ConvertItemTypeToTabIndex(AddedItemInfo.ItemKey.Type);

	FInventoryItemInfo InvenItemInfo = FInventoryItemInfo::Create(AddedItemInfo, SlotIndex);
	UpdateItemInfo(InvenItemInfo);

	if (CurrentTabIndex == TabIndex)
	{
		UpdateWidgetItemSlot(SlotIndex);
	}
}

void UInventoryWidgetBase::SwapItem(int SrcSlotIndex, int DstSlotIndex)
{
	FInventoryItemInfo SrcItemInfo = GetItemInfo(CurrentTabIndex, SrcSlotIndex);
	SrcItemInfo.SlotIndex = DstSlotIndex;

	FInventoryItemInfo DstItemInfo = GetItemInfo(CurrentTabIndex, DstSlotIndex);
	DstItemInfo.SlotIndex = SrcSlotIndex;

	UpdateItemInfo(DstItemInfo);
	UpdateWidgetItemSlot(DstItemInfo.SlotIndex);

	UpdateItemInfo(SrcItemInfo);
	UpdateWidgetItemSlot(SrcItemInfo.SlotIndex);
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

	Character->AddCash(2000);
	UpdateWidgetCash();
}