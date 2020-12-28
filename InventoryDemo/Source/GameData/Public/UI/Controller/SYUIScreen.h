// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYUIBase.h"
#include "SYUIScreen.generated.h"

/**
 * 
 */
class USYWidgetScreen;
class USYWidgetBase;

UCLASS()
class GAMEDATA_API USYUIScreen : public USYUIBase
{
	GENERATED_BODY()

public:
	virtual void Init() final;

private:
	UPROPERTY()
	USYWidgetScreen* StoreWidget;
};
