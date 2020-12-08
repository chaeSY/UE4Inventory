// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemInfo.h"
#include "SYCharacter.generated.h"

struct FItemInfo;

UCLASS()
class GAMEDATA_API ASYCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASYCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int	 GetCash();
	void AddCash(int InAddedCash);

	bool TryBuyItem(FItemKey ItemKey, int ItemCount, int ItemPrice);
	bool TrySellItem(int TabIndex, int ItemCount, int ItemPrice, int SlotIndex);

private:
	void OnKeyPress_Shop();
	void OnKeyPress_Inventory();

private:
	int Cash = 0;
};
