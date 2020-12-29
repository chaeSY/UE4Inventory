// Fill out your copyright notice in the Description page of Project Settings.

#include "SYWidgetLayout.h"
#include "SYWidgetBase.h"

USYWidgetBase* USYWidgetLayout::GetWidget(EUINumber UINumber)
{
	return WidgetMap.FindRef(UINumber);
}

void USYWidgetLayout::NativeConstruct()
{
	BindWidgetFromWidgetName(TEXT("Inventory"));
	BindWidgetFromWidgetName(TEXT("Store"));
}

void USYWidgetLayout::BindWidgetFromWidgetName(FString WidgetName)
{
	USYWidgetBase* Widget = Cast<USYWidgetBase>(GetWidgetFromName(*WidgetName));
	if (Widget)
	{
		WidgetMap.Add(Widget->UINumber, Widget);
	}
}
