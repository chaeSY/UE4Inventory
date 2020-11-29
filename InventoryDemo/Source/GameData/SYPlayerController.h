// Fillout your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATA_API ASYPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnyWhere)
	TSubclassOf<class UWidgetManager> WidgetManagerClass;

	UPROPERTY()
	class UWidgetManager* WidgetManager;

	virtual void BeginPlay() override;
};
