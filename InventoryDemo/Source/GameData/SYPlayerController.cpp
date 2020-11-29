// Fill out your copyright notice in the Description page of Project Settings.


#include "SYPlayerController.h"
#include "UI/ItemSlotWidgetBase.h"
#include "UI/InventoryWidgetBase.h"
#include "UI/WidgetManager.h"

void ASYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetManagerClass)
	{
		WidgetManager = CreateWidget<UWidgetManager>(this, WidgetManagerClass);
		WidgetManager->AddToViewport();
	}
	
	bShowMouseCursor = true;
}
