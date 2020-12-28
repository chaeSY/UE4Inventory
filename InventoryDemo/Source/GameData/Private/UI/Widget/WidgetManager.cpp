// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"
#include "InventoryWidgetBase.h"
#include "StoreWidgetBase.h"
#include "SYUIStore.h"
#include "Components/CanvasPanelSlot.h"
#include "Math/NumericLimits.h"

USYWidgetBase* UWidgetManager::GetWidget(EUINumber UINumber)
{
	return UIMap.FindRef(UINumber).Widget;
}

USYUIBase* UWidgetManager::GetController(EUINumber UINumber)
{
	return UIMap.FindRef(UINumber).Controller;
}

bool UWidgetManager::IsVisible(EUINumber UINumber)
{
	USYWidgetBase* Widget = UIMap.FindRef(UINumber).Widget;
	if (Widget && Widget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}

	return false;
}

void UWidgetManager::ShowUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = UIMap.FindRef(UINumber).Widget;
	if (Widget)
	{
		VisibleUI.AddUnique(UINumber);
		Widget->SetZOrder(NextZOrder++);
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWidgetManager::HideUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = UIMap.FindRef(UINumber).Widget;
	if (Widget)
	{
		VisibleUI.Remove(UINumber);
		if (VisibleUI.Num() == 0)
			ResetNextZOrder();

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
		FWidgetAndController& WidgetAndController = UIMap.FindOrAdd(Widget->UINumber);
		WidgetAndController.Widget = Widget;
	}
	
	Widget = Cast<USYWidgetBase>(GetWidgetFromName(TEXT("Store")));
	if (Widget)
	{
		FWidgetAndController& WidgetAndController = UIMap.FindOrAdd(Widget->UINumber);
		WidgetAndController.Widget = Widget;
	}

	for(const auto& Pair: ControllerClassMap)
	{
		USYUIBase* UIBase = NewObject<USYUIBase>(this, Pair.Value);
		if (UIBase)
		{
			UIBase->Init();
			
			FWidgetAndController& WidgetAndController = UIMap.FindOrAdd(Pair.Key);
			WidgetAndController.Controller = UIBase;
		}
	}
}

void UWidgetManager::SetNextZOrderToUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = UIMap.FindRef(UINumber).Widget;
	if (!Widget)
		return;

	if (TNumericLimits<int32>::Max() <= NextZOrder)
	{
		SortVisibleWidgetByZOrder();
	}

	Widget->SetZOrder(NextZOrder++);
}

void UWidgetManager::ResetNextZOrder()
{
	NextZOrder = FixedZOrder_End;
}

bool UWidgetManager::SortByZOrder(EUINumber UINumber1, EUINumber UINumber2)
{
	USYWidgetBase* Widget1 = GetWidget(UINumber1);
	USYWidgetBase* Widget2 = GetWidget(UINumber2);
	if (!Widget1 || !Widget2)
		return false;

	UCanvasPanelSlot* Canvas1 = Cast<UCanvasPanelSlot>(Widget1->Slot);
	UCanvasPanelSlot* Canvas2 = Cast<UCanvasPanelSlot>(Widget2->Slot);
	if (!Canvas1 || !Canvas2)
		return false;

	return Canvas1->GetZOrder() < Canvas2->GetZOrder();
}

void UWidgetManager::SortVisibleWidgetByZOrder()
{
	VisibleUI.Sort([this](EUINumber UINumber1, EUINumber UINumber2) {
		return this->SortByZOrder(UINumber1, UINumber2);
	});

	for (int i = 0; i < VisibleUI.Num(); ++i)
	{
		USYWidgetBase* Widget = GetWidget(VisibleUI[i]);
		if (Widget)
		{
			int NewZOrder = FixedZOrder_End + i;
			Widget->SetZOrder(NewZOrder);
		}
	}

	NextZOrder = FixedZOrder_End + VisibleUI.Num();
}

