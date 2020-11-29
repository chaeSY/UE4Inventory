// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "InventoryWidgetBase.h"
#include "Operation/DragDropSlot.h"
#include "Operation/ButtonDownSlot.h"
#include "StoreWidgetBase.h"

USYWidgetBase* UWidgetManager::GetWidget(EUINumber UINumber)
{
	return UIMap.FindRef(UINumber);
}

bool UWidgetManager::IsVisible(EUINumber UINumber)
{
	USYWidgetBase* Widget = UIMap.FindRef(UINumber);
	if (Widget && Widget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}

	return false;
}

void UWidgetManager::ShowUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = UIMap.FindRef(UINumber);
	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Visible);
	}

}

void UWidgetManager::HideUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = UIMap.FindRef(UINumber);
	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidgetManager::NativeConstruct()
{
	Visibility = ESlateVisibility::Visible;
	BindWidget();
}

void UWidgetManager::BindWidget()
{
	USYWidgetBase* Widget = nullptr;
	Widget = Cast<UInventoryWidgetBase>(GetWidgetFromName(TEXT("Inventory")));
	if (Widget)
		UIMap.Add(EUINumber::Inventory, Widget);

	Widget = Cast<UStoreWidgetBase>(GetWidgetFromName(TEXT("Store")));
	if (Widget)
		UIMap.Add(EUINumber::Store, Widget);
}

void UWidgetManager::DragDrop(UDragDropOperation* InDragDropOp)
{
	if (InDragDropOp->IsA(UDragDropSlot::StaticClass()))
	{
		UDragDropSlot* DragDropOp = Cast<UDragDropSlot>(InDragDropOp);
		if (!DragDropOp)
			return;

		USYWidgetBase* Widget = UIMap.FindRef(DragDropOp->FromUINumber);
		if (!Widget)
			return;

		Widget->OnDragDrop(DragDropOp);
	}
}