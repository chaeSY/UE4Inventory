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
	DECLARE_EVENT_OneParam(USYWidgetBase, FSYDragDropEvent, UDragDropOperation*);
	FSYDragDropEvent OnDragDropEvent;

	DECLARE_EVENT_OneParam(USYWidgetBase, FSYMouseButtonDownEvent, USYMouseButtonDownOp*);
	FSYMouseButtonDownEvent OnMouseButtonDownEvent;

protected:
	virtual auto CreateSrcDragDropOp() -> UDragDropOperation*;
	virtual void SetDstDragDropOp(UDragDropOperation* OutDragDropOp);
	virtual auto CreateButtonDownOp(FKey key) ->USYMouseButtonDownOp*;

private:
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) final;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) final;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) final;

protected:
	EUINumber TempUINumber;
	bool	  bUseDragDrop;
	bool	  bUseMouseButtonDown;
};
