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
	DECLARE_EVENT_OneParam(ASYCharacter, FUpdateCashEvent, int32);	// Param1: Cash

public:
	ASYCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FUpdateCashEvent& OnUpdateCash();
	int	 GetCash();
	void UpdateCash(int NewCash);

	bool TryBuyItem(FItemKey ItemKey, int ItemCount, int ItemPrice);
	bool TrySellItem(int TabIndex, int ItemCount, int ItemPrice, int SlotIndex);

private:
	void OnKeyPress_Shop();
	void OnKeyPress_Inventory();

private:
	FUpdateCashEvent UpdateCashEvent;
	int Cash = 0;
};
