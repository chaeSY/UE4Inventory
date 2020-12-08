// Fill out your copyright notice in the Description page of Project Settings.
#include "SYCharacter.h"
#include "UI/InventoryWidgetBase.h"
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

bool ASYCharacter::TryBuyItem(FItemKey ItemKey, int ItemCount, int ItemPrice)
{
	UInventoryWidgetBase* InventoryWidget = SYUtil::GetWidget<UInventoryWidgetBase>(GetWorld(), EUINumber::Inventory);
	if(!InventoryWidget)
		return false;

	int ResultPrice = ItemPrice * ItemCount;
	if (Cash < ResultPrice)
		return false;

	if (InventoryWidget->TryAddItem(ItemKey, ItemCount))
	{
		Cash -= ResultPrice;
		InventoryWidget->OnBuyItem();
		return true;
	}

	return false;
}

bool ASYCharacter::TrySellItem(int TabIndex, int ItemCount, int ItemPrice, int SlotIndex)
{
	UInventoryWidgetBase* InventoryWidget = SYUtil::GetWidget<UInventoryWidgetBase>(GetWorld(), EUINumber::Inventory);
	if (!InventoryWidget)
		return false;

	int ResultPrice = ItemPrice * ItemCount;
	if (InventoryWidget->TrySubtractItem(TabIndex, ItemCount, SlotIndex))
	{
		Cash += ResultPrice;
		InventoryWidget->OnBuyItem();
		return true;
	}

	return false;
}
