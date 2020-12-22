// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "UIOperation.h"
#include "SYCharacter.h"
#include "SYUtil.h"
#include "SYInteractionWidgetItemSlot.h"

bool UInventoryWidgetBase::TryAddItem(FItemKey ItemKey, int ItemCount)
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

bool UInventoryWidgetBase::TrySubtractItem(int TabIndex, int SubtractCount, int SlotIndex)
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

bool UInventoryWidgetBase::TrySubtractItemInSlotOrder(const FItemInfo& SubtractItemInfo, int SubtractCount)
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

//void UInventoryWidgetBase::OnDragDrop(UDragDropOperation* InDragDropOp)
//{
//	if (InDragDropOp->IsA(USYSlotDragDropOp::StaticClass()))
//	{
//		USYSlotDragDropOp* DragDrop = Cast<USYSlotDragDropOp>(InDragDropOp);
//		if (DragDrop->DstUINumber == EUINumber::Inventory)
//		{
//			if (DragDrop->SrcSlotIndex == DragDrop->DstSlotIndex)
//				return;
//
//			const FInventoryItemInfo& SrcItemInfo = GetItemInfo(CurrentTabIndex, DragDrop->SrcSlotIndex);
//			const FInventoryItemInfo& DstItemInfo = GetItemInfo(CurrentTabIndex, DragDrop->DstSlotIndex);
//			bool IsEmptySrcSlot = SrcItemInfo.ItemKey.ID == 0;
//			bool IsEmptyDstSlot = DstItemInfo.ItemKey.ID == 0;
//			if (!IsEmptySrcSlot)
//			{
//				if (IsEmptyDstSlot)
//				{
//					FInventoryItemInfo NewItemInfo = FItemInfoFactory::CreateInventoryItemInfo(GetWorld(), SrcItemInfo.ItemKey, SrcItemInfo.Count, DragDrop->DstSlotIndex);
//					AddItem(NewItemInfo);
//					RemoveItem(CurrentTabIndex, DragDrop->SrcSlotIndex);
//				}
//				else // !IsEmptyDstSlot
//				{
//					SwapItem(DragDrop->SrcSlotIndex, DragDrop->DstSlotIndex);
//				}
//			}
//		}
//	}
//}



void UInventoryWidgetBase::OnButtonDown(class USYMouseButtonDownOp* InButtonDownOp)
{
	if (InButtonDownOp->IsA(USYSlotMouseButtonDownOp::StaticClass()))
	{
		USYSlotMouseButtonDownOp* ButtonDownOp = Cast<USYSlotMouseButtonDownOp>(InButtonDownOp);
		if (ButtonDownOp->PressedKey == EKeys::RightMouseButton)
		{
			UWidgetManager* WidgetManager = SYUtil::GetWidgetManager(GetWorld());
			if (WidgetManager && WidgetManager->IsVisible(EUINumber::Store))
			{
				ASYCharacter* Character = GetOwningPlayerPawn<ASYCharacter>();
				if (!Character)
					return;
				
				const FInventoryItemInfo& ItemInfo = GetItemInfo(CurrentTabIndex, ButtonDownOp->SlotIndex);
				Character->TrySellItem(CurrentTabIndex, ItemInfo.Count, ItemInfo.Price, ItemInfo.SlotIndex);
			}
			else
			{
				//test
				TrySubtractItem(CurrentTabIndex, 1, ButtonDownOp->SlotIndex);
			}
		}
	}
}

void UInventoryWidgetBase::BindWidget()
{
	// binding ItemSlotWidgetList
	for (int InvenSlotIdex = 0; InvenSlotIdex < InventorySlotCount; ++InvenSlotIdex)
	{
		FString WidgetName = FString::Printf(TEXT("InventorySlot_%d"), InvenSlotIdex);
		USYInteractionWidgetItemSlot* ItemSlotWidget = Cast<USYInteractionWidgetItemSlot>(GetWidgetFromName(*WidgetName));
		if (ItemSlotWidget)
		{
			ItemSlotWidget->SetSlotIndex(InvenSlotIdex);
			ItemSlotWidget->SetParentUINumber(EUINumber::Inventory);
			ItemSlotWidget->OnDragDrop().AddUFunction(this, FName("OnDragDropInternal"));
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

	USYInteractionWidgetItemSlot* SlotWidget = ItemSlotWidgetList[SlotIndex];
	if (!SlotWidget)
		return;

	const FItemInfo& ItemInfo = GetItemInfo(CurrentTabIndex, SlotIndex);
	SlotWidget->UpdateSlot(ItemInfo);
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
	if (CashText)
	{
		ASYCharacter* Character = GetOwningPlayerPawn<ASYCharacter>();
		if (Character)
		{
			int Cash = Character->GetCash();
			CashText->SetText(FText::AsNumber(Cash));
		}
	}
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

auto UInventoryWidgetBase::GetItemInfo(int TabIndex, int SlotIndex) ->const FInventoryItemInfo&
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

void UInventoryWidgetBase::RemoveItemInfo(int TabIndex, int SlotIndex)
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

bool UInventoryWidgetBase::IsStackableItem(FItemKey ItemKey)
{
	FSYTableItemBase* ItemTable = SYUtil::GetGameData<FSYTableItemBase>(GetWorld(), ItemKey.GetTableType(), ItemKey.ID);
	if (ItemTable && ItemTable->MaxStackCount > 0)
		return true;

	return false;
}

bool UInventoryWidgetBase::CanAddItem(FItemKey ItemKey, int ItemCount)
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

void UInventoryWidgetBase::AddItem(const FInventoryItemInfo& AddedItemInfo)
{
	UpdateItemInfo(AddedItemInfo);

	int TabIndex = ConvertItemTypeToTabIndex(AddedItemInfo.ItemKey.Type);
	if (CurrentTabIndex == TabIndex)
	{
		UpdateWidgetItemSlot(AddedItemInfo.SlotIndex);
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
	ASYCharacter* Character = GetOwningPlayerPawn<ASYCharacter>();
	if(!Character)
		return;

	Character->AddCash(2000);
	UpdateWidgetCash();
}

void UInventoryWidgetBase::OnDragDropInternal(EUINumber SrcUINumber, int32 InventorySlotIndex, EUINumber DstUINumber, int32 DstSlotIndex)
{
	DragDropEvent.Broadcast(SrcUINumber, InventorySlotIndex, DstUINumber, DstSlotIndex);
}
