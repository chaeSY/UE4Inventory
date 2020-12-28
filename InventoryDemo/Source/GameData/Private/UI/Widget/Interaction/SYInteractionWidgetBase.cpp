// Fill out your copyright notice in the Description page of Project Settings.


#include "SYInteractionWidgetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "SYUtil.h"
#include "SYWidgetBase.h"

FReply USYInteractionWidgetBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown: %s"), *Name);

	FEventReply Reply;

	// drag
	{
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	// buttondown
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			OnMouseLButtonDownInternal();
		}
		else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			OnMouseRButtonDownInternal();
		}
	}

	return Reply.NativeReply;
}

void USYInteractionWidgetBase::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	USYWidgetBase* ParentWidget = SYUtil::GetWidget(GetWorld(), ParentUINumber);
	if (!ParentWidget)
		return;

	UDragDropOperation* op = NewObject<UDragDropOperation>();
	op->OnDragged.AddDynamic(this, &USYInteractionWidgetBase::OnDragging);
	op->OnDrop.AddDynamic(this, &USYInteractionWidgetBase::OnDragDropInternal);
	op->Offset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	op->Payload = CreatePayload();
	
	OutOperation = op;
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected: %s"), *Name);
}

bool USYInteractionWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop: %s"), *Name);
	
	if (InOperation && InOperation->Payload && InOperation->Payload->IsA(UDragDropPayloadBase::StaticClass()))
	{
		SetPayloadOnDrop(Cast<UDragDropPayloadBase>(InOperation->Payload));
	}

	return true;
}

void USYInteractionWidgetBase::OnDragDropInternal(UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDragDrop: %s"), *Name);
}

void USYInteractionWidgetBase::OnDragging(UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDragging: %s"), *Name);
}