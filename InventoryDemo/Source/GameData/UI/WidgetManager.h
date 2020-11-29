// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "WidgetManager.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EUINumber : uint8
{
	None = 0		UMETA(Display = "None"),
	Game = 1		UMETA(Display = "Game"),
	Inventory = 2	UMETA(Display = "Inventory"),
	Store = 3		UMETA(Display = "Store"),
};

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

	void DragDrop(class UDragDropOperation* DragDropOp);

private:
	virtual void NativeConstruct() override;
	void BindWidget();

private:
	UPROPERTY()
	TMap<EUINumber, USYWidgetBase*> UIMap;
};
