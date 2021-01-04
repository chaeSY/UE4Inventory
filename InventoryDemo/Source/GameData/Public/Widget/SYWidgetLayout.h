// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYDefine.h"
#include "SYWidgetLayout.generated.h"

/**
 * 
 */

class USYWidgetBase;

UCLASS()
class GAMEDATA_API USYWidgetLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USYWidgetBase* GetWidget(EUINumber UINumber);
	
	template<typename T>
	T* GetWidget(EUINumber UINumber)
	{
		return Cast<T>(GetWidget(UINumber));
	}

private:
	virtual void NativeConstruct() final;
	void BindWidgetFromWidgetName(FString WidgetName);

private:
	UPROPERTY()
	TMap<EUINumber, USYWidgetBase*> WidgetMap;
};
