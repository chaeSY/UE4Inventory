// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYDefine.h"
#include "SYWidgetBase.generated.h"

/**
 * 
 */
class UDragDropOperation;
class USYMouseButtonDownOp;
class USYDragDropOp;

UCLASS(Blueprintable, BlueprintType)
class GAMEDATA_API USYWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	EUINumber UINumber;
};
