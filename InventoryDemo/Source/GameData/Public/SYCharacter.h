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
	DECLARE_EVENT_OneParam(ASYCharacter, FUpdateCashEvent, int32 /* Cash */);

public:
	ASYCharacter();

	FUpdateCashEvent& OnUpdateCash();
	int	 GetCash();
	void UpdateCash(int NewCash);

	bool TryBuyItem(FItemKey ItemKey, int ItemCount, int ItemPrice);
	bool TrySellItem(int TabIndex, int ItemCount, int ItemPrice, int SlotIndex);

private:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void OnKeyPress_Shop();
	void OnKeyPress_Inventory();

private:
	FUpdateCashEvent UpdateCashEvent;
	int Cash = 0;
};
