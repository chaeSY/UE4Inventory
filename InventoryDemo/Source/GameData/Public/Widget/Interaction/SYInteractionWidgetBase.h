// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYDefine.h"
#include "SYInteractionWidgetBase.generated.h"

/**
 * 
 */

UCLASS()
class UDragDropPayloadBase : public UObject
{
	GENERATED_BODY()

public:
	EUINumber SrcUINumber;
	EUINumber DstUINumber;
};

class UDragDropOperation;
class USYUIManager;
class USYWidgetBase;

UCLASS(Blueprintable, BlueprintType)
class GAMEDATA_API USYInteractionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	virtual void OnDragDropInternal(UDragDropOperation* InOperation);

	UFUNCTION()
	virtual void OnDragging(UDragDropOperation* InOperation);

	virtual void OnMouseLButtonDownInternal() {}
	virtual void OnMouseRButtonDownInternal() {}

	virtual auto CreatePayload()->UDragDropPayloadBase* { return nullptr; }
	virtual void SetPayloadOnDrop(UDragDropPayloadBase* Payload) {}

	USYWidgetBase* GetWidget();

private:
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	EUINumber ParentUINumber;
};
