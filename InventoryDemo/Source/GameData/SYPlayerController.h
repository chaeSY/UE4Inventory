// Fillout your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SYPlayerController.generated.h"

/**
 * 
 */
class UWidgetManager;
UCLASS()
class GAMEDATA_API ASYPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UWidgetManager* GetWidgetManager();

private:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnyWhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWidgetManager> WidgetManagerClass;

	UPROPERTY()
	UWidgetManager* WidgetManager;
};
