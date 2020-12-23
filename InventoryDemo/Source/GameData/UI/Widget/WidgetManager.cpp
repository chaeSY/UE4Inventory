// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "UIOperation.h"
#include "InventoryWidgetBase.h"
#include "StoreWidgetBase.h"
#include "SYUIStore.h"

USYWidgetBase* UWidgetManager::GetWidget(EUINumber UINumber)
{
	//return UIMap.FindRef(UINumber);
	return UIMap2.FindRef(UINumber).Widget;
}

USYUIBase* UWidgetManager::GetController(EUINumber UINumber)
{
	return UIMap2.FindRef(UINumber).Controller;
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
	Widget = Cast<USYWidgetBase>(GetWidgetFromName(TEXT("Inventory")));
	if (Widget)
	{
		UIMap.Add(EUINumber::Inventory, Widget);
		
		FWidgetAndController& WidgetAndController = UIMap2.FindOrAdd(Widget->UINumber);
		WidgetAndController.Widget = Widget;
	}

	Widget = Cast<USYWidgetBase>(GetWidgetFromName(TEXT("Store")));
	if (Widget)
	{
		UIMap.Add(EUINumber::Store, Widget);

		FWidgetAndController& WidgetAndController = UIMap2.FindOrAdd(Widget->UINumber);
		WidgetAndController.Widget = Widget;
	}


	//FWidgetAndController& WidgetAndController = UIMap2.FindOrAdd(Widget->GetUINumber());
	//WidgetAndController.Widget = Widget;

	for(const auto& Pair: ControllerClassMap)
	{
		USYUIBase* UIBase = NewObject<USYUIBase>(this, Pair.Value);
		if (UIBase)
		{
			UIBase->Init();
			
			FWidgetAndController& WidgetAndController = UIMap2.FindOrAdd(Pair.Key);
			WidgetAndController.Controller = UIBase;
		}
	}
}