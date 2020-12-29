// Fill out your copyright notice in the Description page of Project Settings.
#include "SYCharacter.h"
#include "SYUIInventory.h"
#include "SYUtil.h"
#include "SYUIStore.h"
#include "SYUIManager.h"

ASYCharacter::ASYCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASYCharacter::BeginPlay()
{
	Super::BeginPlay();

	// test
	UpdateCash(10000);
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
	USYUIManager* UIManager = SYUtil::GetUIManager(GetWorld());
	if (UIManager)
	{
		if (!UIManager->IsVisible(EUINumber::Store))
		{
			UIManager->ShowUI(EUINumber::Store);
		}
		else
		{
			UIManager->HideUI(EUINumber::Store);
		}
	}
}

void ASYCharacter::OnKeyPress_Inventory()
{
	USYUIManager* UIManager = SYUtil::GetUIManager(GetWorld());
	if (UIManager)
	{
		if (!UIManager->IsVisible(EUINumber::Inventory))
		{
			UIManager->ShowUI(EUINumber::Inventory);
		}
		else
		{
			UIManager->HideUI(EUINumber::Inventory);
		}
	}
}

ASYCharacter::FUpdateCashEvent& ASYCharacter::OnUpdateCash()
{
	return UpdateCashEvent;
}

int ASYCharacter::GetCash()
{
	return Cash;
}

void ASYCharacter::UpdateCash(int NewCash)
{
	Cash = NewCash;
	UpdateCashEvent.Broadcast(Cash);
}

bool ASYCharacter::TryBuyItem(FItemKey ItemKey, int ItemCount, int ItemPrice)
{
	USYUIInventory* Inventory = SYUtil::GetUI<USYUIInventory>(GetWorld(), EUINumber::Inventory);
	if(!Inventory)
		return false;
	
	int FullPrice = ItemPrice * ItemCount;
	if (Cash < FullPrice)
		return false;
	
	if (Inventory->TryAddItem(ItemKey, ItemCount))
	{
		int NewCash = Cash - FullPrice;
		UpdateCash(NewCash);

		return true;
	}

	return false;
}

bool ASYCharacter::TrySellItem(int TabIndex, int ItemCount, int ItemPrice, int SlotIndex)
{
	USYUIInventory* Inventory = SYUtil::GetUI<USYUIInventory>(GetWorld(), EUINumber::Inventory);
	if (!Inventory)
		return false;
	
	int FullPrice = ItemPrice * ItemCount;
	if (Inventory->TrySubtractItem(TabIndex, ItemCount, SlotIndex))
	{
		int NewCash = Cash + FullPrice;
		UpdateCash(NewCash);

		return true;
	}

	return false;
}
