// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYUIBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATA_API USYUIBase : public UObject
{
	GENERATED_BODY()

public:
	void ShowUI();
	void HideUI();

protected:
	class USYWidgetBase* Widget;
};