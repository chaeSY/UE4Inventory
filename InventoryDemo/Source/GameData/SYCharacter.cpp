// Fill out your copyright notice in the Description page of Project Settings.
#include "SYCharacter.h"
#include "SYPlayerController.h"
#include "UI/WidgetManager.h"
#include "UI/InventoryWidgetBase.h"
#include "GameData/SYGameDataManager.h"

ASYCharacter::ASYCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASYCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddCash(10000);
}

void ASYCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &ASYCharacter::OnKeyPress_Inventory);
	PlayerInputComponent->BindAction(TEXT("Shop"), IE_Pressed, this, &ASYCharacter::OnKeyPress_Shop);
}

void ASYCharacter::OnKeyPress_Shop()
{
	ASYPlayerController* SYController = Cast<ASYPlayerController>(GetController());
	if (SYController && SYController->WidgetManager)
	{
		if (!SYController->WidgetManager->IsVisible(EUINumber::Store))
		{
			SYController->WidgetManager->ShowUI(EUINumber::Store);
		}
		else
		{
			SYController->WidgetManager->HideUI(EUINumber::Store);
		}
	}
}

void ASYCharacter::OnKeyPress_Inventory()
{
	ASYPlayerController* SYController = Cast<ASYPlayerController>(GetController());
	if (SYController && SYController->WidgetManager)
	{
		if (!SYController->WidgetManager->IsVisible(EUINumber::Inventory))
		{
			SYController->WidgetManager->ShowUI(EUINumber::Inventory);
		}
		else
		{
			SYController->WidgetManager->HideUI(EUINumber::Inventory);
		}
	}
}

int ASYCharacter::GetCash()
{
	return Cash;
}

void ASYCharacter::AddCash(int InAddedCash)
{
	Cash += InAddedCash;
}

bool ASYCharacter::TryBuyItem(const FItemInfo& BuyItemInfo)
{
	ASYPlayerController* SYController = Cast<ASYPlayerController>(GetController());
	if (!SYController || !SYController->WidgetManager)
		return false;

	UInventoryWidgetBase* InventoryWidget = SYController->WidgetManager->GetWidget<UInventoryWidgetBase>(EUINumber::Inventory);
	if(!InventoryWidget)
		return false;

	if (Cash < BuyItemInfo.Price)
		return false;

	if (InventoryWidget->TryAddItem(BuyItemInfo))
	{
		Cash -= BuyItemInfo.Price * BuyItemInfo.Count;
		InventoryWidget->OnBuyItem();
		return true;
	}

	return false;
}

bool ASYCharacter::TrySellItem(const FItemInfo& SellItemInfo)
{
	ASYPlayerController* SYController = Cast<ASYPlayerController>(GetController());
	if (!SYController || !SYController->WidgetManager)
		return false;

	UInventoryWidgetBase* InventoryWidget = SYController->WidgetManager->GetWidget<UInventoryWidgetBase>(EUINumber::Inventory);
	if (!InventoryWidget)
		return false;

	int SellItemPrice = SellItemInfo.Price;
	int SellItemCount = SellItemInfo.Count;
	if (InventoryWidget->TrySubtractItemInSlotOrder(SellItemInfo, SellItemInfo.Count))
	{
		Cash += SellItemPrice * SellItemCount;
		InventoryWidget->OnBuyItem();
		return true;
	}

	return false;
}
