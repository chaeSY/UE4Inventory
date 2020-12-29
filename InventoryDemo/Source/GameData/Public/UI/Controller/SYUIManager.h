// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYDefine.h"
#include "SYUIManager.generated.h"

/**
 * 
 */

class USYUIBase;
class USYWidgetBase;
class USYWidgetLayout;
class USYInteractionWidgetBase;

UCLASS(Blueprintable)
class GAMEDATA_API USYUIManager : public UObject
{
	GENERATED_BODY()

public:
	USYUIBase* GetUI(EUINumber UINumber);

	template<typename T>
	T* GetUI(EUINumber UINumber)
	{
		return Cast<T>(GetUI(UINumber));
	}

	void InitUIManager();
	bool IsVisible(EUINumber UINumber);
	void ShowUI(EUINumber UINumber);
	void HideUI(EUINumber UINumber);

	void SetNextZOrderToUI(EUINumber UINumber);

private:
	friend USYInteractionWidgetBase;

	USYWidgetBase* GetWidgetInternal(EUINumber UINumber);

	enum FixedZOrder { FixedZOrder_Screen = 0, FixedZOrder_End };
	void ResetNextZOrder();
	void SortVisibleWidgetByZOrder();
	bool SortByZOrder(EUINumber UINumber1, EUINumber UINumber2);

private:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USYWidgetLayout> LayoutWidgetClass;

	UPROPERTY()
	USYWidgetLayout* LayoutWidget;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EUINumber, TSubclassOf<USYUIBase>> UIClassMap;

	UPROPERTY()
	TMap<EUINumber, USYUIBase*> UIMap;

	UPROPERTY()
	TArray<EUINumber> VisibleUI; // screen¿∫ ¡¶ø‹


	int NextZOrder = FixedZOrder_End;
};
