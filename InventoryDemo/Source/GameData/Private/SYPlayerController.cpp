// Fill out your copyright notice in the Description page of Project Settings.


#include "SYPlayerController.h"
#include "SYUIManager.h"
#include "SYWidgetLayout.h"

void ASYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UIManagerClass)
	{
		UIManager = NewObject<USYUIManager>(this, UIManagerClass);
		if (UIManager)
			UIManager->InitUIManager();
	}

	bShowMouseCursor = true;
}


