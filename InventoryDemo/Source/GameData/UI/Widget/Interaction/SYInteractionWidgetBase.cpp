// Fill out your copyright notice in the Description page of Project Settings.


#include "SYInteractionWidgetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "SYUtil.h"
#include "UIOperation.h"

FReply USYInteractionWidgetBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown: %s"), *Name);
	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return Reply.NativeReply;
}

void USYInteractionWidgetBase::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragDropOperation* op = NewObject<UDragDropOperation>();
	op->OnDragged.AddDynamic(this, &USYInteractionWidgetBase::OnDragging);
	op->OnDrop.AddDynamic(this, &USYInteractionWidgetBase::OnDragDrop);
	op->Offset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	op->Payload = CreatePayload();
	
	OutOperation = op;
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected: %s"), *Name);
}

bool USYInteractionWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: %s"), *Name);
	
	if (InOperation && InOperation->Payload)
	{
		SetPayloadOnDrop(InOperation->Payload);
	}

	return true;
}

void USYInteractionWidgetBase::OnDragDrop(UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDragDrop: %s"), *Name);
}

void USYInteractionWidgetBase::OnDragging(UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDragging: %s"), *Name);
}