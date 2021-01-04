// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYDefine.h"
#include "SYUIBase.generated.h"

/**
 * 
 */
class USYWidgetBase;

UCLASS()
class GAMEDATA_API USYUIBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(EUINumber InUINumber, USYWidgetBase* InWidget);

protected:
	EUINumber UINumber;
	USYWidgetBase* Widget;
};