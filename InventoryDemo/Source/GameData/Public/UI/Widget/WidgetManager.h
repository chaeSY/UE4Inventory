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

	void SetNextZOrderToUI(EUINumber UINumber);

private:
	virtual void NativeConstruct() final;
	void BindWidget();

	enum FixedZOrder { FixedZOrder_Screen = 0, FixedZOrder_End };
	void ResetNextZOrder();
	void SortVisibleWidgetByZOrder();
	bool SortByZOrder(EUINumber UINumber1, EUINumber UINumber2);

private:
	UPROPERTY()
	TMap<EUINumber, FWidgetAndController> UIMap;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EUINumber, TSubclassOf<USYUIBase>> ControllerClassMap;

	UPROPERTY()
	TArray<EUINumber> VisibleUI; // screen¿∫ ¡¶ø‹

	int NextZOrder = FixedZOrder_End;
};
