// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemDefine.h"
#include "SYCharacter.generated.h"

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

	bool TryBuyItem(FItemInfo InItemInfo);
	bool TrySellItem();

private:
	//key event
	void OnKeyPress_Shop();
	void OnKeyPress_Inventory();

private:
	int Cash = 0;
};
