// Fill out your copyright notice in the Description page of Project Settings.


#include "SYPlayerController.h"
#include "UI/Widget/WidgetManager.h"


UWidgetManager* ASYPlayerController::GetWidgetManager()
{
	return WidgetManager;
}

void ASYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetManagerClass)
	{
		WidgetManager = CreateWidget<UWidgetManager>(this, WidgetManagerClass);
		if (WidgetManager)
		{
			WidgetManager->AddToViewport();
		}
	}
	
	bShowMouseCursor = true;
}
