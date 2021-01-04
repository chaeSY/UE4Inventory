// Fill out your copyright notice in the Description page of Project Settings.


#include "SYUIManager.h"
#include "InventoryWidgetBase.h"
#include "StoreWidgetBase.h"
#include "SYUIStore.h"
#include "SYWidgetLayout.h"
#include "Components/CanvasPanelSlot.h"
#include "Math/NumericLimits.h"
#include "SYPlayerController.h"
#include "SYUtil.h"

USYUIBase* USYUIManager::GetUI(EUINumber UINumber)
{
	return UIMap.FindRef(UINumber);
}

bool USYUIManager::IsVisible(EUINumber UINumber)
{
	USYWidgetBase* Widget = GetWidgetInternal(UINumber);
	if (Widget && Widget->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}

	return false;
}

void USYUIManager::ShowUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = GetWidgetInternal(UINumber);
	if (Widget)
	{
		VisibleUI.AddUnique(UINumber);
		Widget->SetZOrder(NextZOrder++);
		Widget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USYUIManager::HideUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = GetWidgetInternal(UINumber);
	if (Widget)
	{
		VisibleUI.Remove(UINumber);
		if (VisibleUI.Num() == 0)
			ResetNextZOrder();

		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYUIManager::InitUIManager()
{
	if (LayoutWidgetClass)
	{
		if (LayoutWidgetClass)
		{
			LayoutWidget = CreateWidget<USYWidgetLayout>(SYUtil::GetPlayerController(GetWorld()), LayoutWidgetClass);
			if (LayoutWidget)
				LayoutWidget->AddToViewport();
		}
	}

	for (const auto& Pair : UIClassMap)
	{
		USYUIBase* UIBase = NewObject<USYUIBase>(this, Pair.Value);
		if (UIBase)
		{
			UIBase->Init(Pair.Key, GetWidgetInternal(Pair.Key));
			UIMap.Add(Pair.Key, UIBase);
		}
	}
}

void USYUIManager::SetNextZOrderToUI(EUINumber UINumber)
{
	USYWidgetBase* Widget = GetWidgetInternal(UINumber);
	if (!Widget)
		return;

	if (TNumericLimits<int32>::Max() <= NextZOrder)
	{
		SortVisibleWidgetByZOrder();
	}

	Widget->SetZOrder(NextZOrder++);
}

void USYUIManager::ResetNextZOrder()
{
	NextZOrder = FixedZOrder_End;
}

bool USYUIManager::SortByZOrder(EUINumber UINumber1, EUINumber UINumber2)
{
	USYWidgetBase* Widget1 = GetWidgetInternal(UINumber1);
	USYWidgetBase* Widget2 = GetWidgetInternal(UINumber2);
	if (!Widget1 || !Widget2)
		return false;

	UCanvasPanelSlot* Canvas1 = Cast<UCanvasPanelSlot>(Widget1->Slot);
	UCanvasPanelSlot* Canvas2 = Cast<UCanvasPanelSlot>(Widget2->Slot);
	if (!Canvas1 || !Canvas2)
		return false;

	return Canvas1->GetZOrder() < Canvas2->GetZOrder();
}

void USYUIManager::SortVisibleWidgetByZOrder()
{
	VisibleUI.Sort([this](EUINumber UINumber1, EUINumber UINumber2) {
		return this->SortByZOrder(UINumber1, UINumber2);
	});

	for (int i = 0; i < VisibleUI.Num(); ++i)
	{
		USYWidgetBase* Widget = GetWidgetInternal(VisibleUI[i]);
		if (Widget)
		{
			int NewZOrder = FixedZOrder_End + i;
			Widget->SetZOrder(NewZOrder);
		}
	}

	NextZOrder = FixedZOrder_End + VisibleUI.Num();
}

USYWidgetBase* USYUIManager::GetWidgetInternal(EUINumber UINumber)
{
	if (LayoutWidget)
	{
		return LayoutWidget->GetWidget(UINumber);
	}
	
	return nullptr;
}
