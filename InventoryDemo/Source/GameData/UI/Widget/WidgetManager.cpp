// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "UIOperation.h"
#include "InventoryWidgetBase.h"
#include "StoreWidgetBase.h"
#include "SYUIStore.h"

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

	if (StoreClass)
	{
		UIStore = NewObject<USYUIStore>(this, StoreClass);
		if(UIStore)
			UIStore->Bind();
	}
	

}