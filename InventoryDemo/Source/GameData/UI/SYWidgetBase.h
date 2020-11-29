// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GAMEDATA_API USYWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnDragDrop(class UDragDropOperation* DragDropOp); // 드래그를 시작한 UI의 OnDragDrop을 호출합니다.
	virtual void OnButtonDown(class UButtonDownOperation* buttondown);
};
