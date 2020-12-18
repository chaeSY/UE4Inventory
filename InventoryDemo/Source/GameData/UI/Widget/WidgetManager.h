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

	bool IsVisible(EUINumber UINumber);
	void ShowUI(EUINumber UINumber);
	void HideUI(EUINumber UINumber);

private:
	virtual void NativeConstruct() final;
	void BindWidget();

private:
	UPROPERTY()
	TMap<EUINumber, USYWidgetBase*> UIMap;

	//temp
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USYUIStore> StoreClass;

	UPROPERTY()
	class USYUIStore* UIStore;
};
