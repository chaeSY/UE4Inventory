// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYWidgetBase.h"
#include "SYWidgetScreen.generated.h"

/**
 * 
 */

UCLASS()
class GAMEDATA_API USYWidgetScreen : public USYWidgetBase
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override final;
};
