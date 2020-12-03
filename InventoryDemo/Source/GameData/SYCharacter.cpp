// Fill out your copyright notice in the Description page of Project Settings.
#include "SYCharacter.h"
#include "SYPlayerController.h"
#include "UI/WidgetManager.h"
#include "UI/InventoryWidgetBase.h"
#include "GameData/SYGameDataManager.h"
#include "SYUtil.h"

ASYCharacter::ASYCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASYCharacter::BeginPlay()
{
	Super::BeginPlay();

	// test
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
	UWidgetManager* WidgetManager = SYUtil::GetWidgetManager(GetWorld());
	if (WidgetManager)
	{
		if (!WidgetManager->IsVisible(EUINumber::Store))
		{
			WidgetManager->ShowUI(EUINumber::Store);
		}
		else
		{
			WidgetManager->HideUI(EUINumber::Store);
		}
	}
}

void ASYCharacter::OnKeyPress_Inventory()
{
	UWidgetManager* WidgetManager = SYUtil::GetWidgetManager(GetWorld());
	if (WidgetManager)
	{
		if (!WidgetManager->IsVisible(EUINumber::Inventory))
		{
			WidgetManager->ShowUI(EUINumber::Inventory);
		}
		else
		{
			WidgetManager->HideUI(EUINumber::Inventory);
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
	UInventoryWidgetBase* InventoryWidget = SYUtil::GetWidget<UInventoryWidgetBase>(GetWorld(), EUINumber::Inventory);
	if(!InventoryWidget)
		return false;

	int ResultPrice = BuyItemInfo.Price * BuyItemInfo.Count;
	if (Cash < ResultPrice)
		return false;

	if (InventoryWidget->TryAddItem(BuyItemInfo))
	{
		Cash -= ResultPrice;
		InventoryWidget->OnBuyItem();
		return true;
	}

	return false;
}

bool ASYCharacter::TrySellItem(const FItemInfo& SellItemInfo)
{
	UInventoryWidgetBase* InventoryWidget = SYUtil::GetWidget<UInventoryWidgetBase>(GetWorld(), EUINumber::Inventory);
	if (!InventoryWidget)
		return false;

	int ResultPrice = SellItemInfo.Price * SellItemInfo.Count;
	if (InventoryWidget->TrySubtractItemInSlotOrder(SellItemInfo, SellItemInfo.Count))
	{
		Cash += ResultPrice;
		InventoryWidget->OnBuyItem();
		return true;
	}

	return false;
}
