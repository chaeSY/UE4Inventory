// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYDefine.h"
#include "WidgetManager.generated.h"

/**
 * 
 */

class USYWidgetBase;
class USYUIBase;

USTRUCT()
struct FWidgetAndController
{
	GENERATED_BODY()
	
	USYWidgetBase* Widget = nullptr;
	USYUIBase* Controller = nullptr;
};


UCLASS(Blueprintable, BlueprintType)
class GAMEDATA_API UWidgetManager : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USYWidgetBase* GetWidget(EUINumber UINumber);
	
	template<typename T>
	T* GetWidget(EUINumber UINumber)
	{
		return Cast<T>(GetWidget(UINumber));
	}

	USYUIBase* GetController(EUINumber UINumber);

	template<typename T>
	T* GetController(EUINumber UINumber)
	{
		return Cast<T>(GetController(UINumber));
	}
	
	bool IsVisible(EUINumber UINumber);
	void ShowUI(EUINumber UINumber);
	void HideUI(EUINumber UINumber);

private:
	virtual void NativeConstruct() final;
	void BindWidget();

private:
	UPROPERTY()
	TMap<EUINumber, USYWidgetBase*> UIMap;

	UPROPERTY()
	TMap<EUINumber, FWidgetAndController> UIMap2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EUINumber, TSubclassOf<USYUIBase>> ControllerClassMap;
};