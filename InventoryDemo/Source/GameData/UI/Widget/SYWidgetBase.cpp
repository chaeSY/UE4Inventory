// Fill out your copyright notice in the Description page of Project Settings.


#include "SYWidgetBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SYUtil.h"
#include "UIOperation.h"

FReply USYWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (bUseDragDrop)
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}
	
	if (bUseMouseButtonDown)
	{
		FKey PressedKey;
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			PressedKey = EKeys::LeftMouseButton;
		}
		else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			PressedKey = EKeys::RightMouseButton;
		}
		
		USYWidgetBase* Widget = SYUtil::GetWidget(GetWorld(), TempUINumber);
		USYMouseButtonDownOp* op = CreateButtonDownOp(PressedKey);
		if (Widget && op)
		{
			Widget->OnMouseButtonDownEvent.Broadcast(op);
		}
	}

	return reply.NativeReply;
}

void USYWidgetBase::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	OutOperation = CreateSrcDragDropOp();
}

bool USYWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	SetDstDragDropOp(InOperation);

	USYDragDropOp* DragDropOp = Cast<USYDragDropOp>(InOperation);
	if (!DragDropOp)
		return false;

	USYWidgetBase* Widget = SYUtil::GetWidget(GetWorld(), DragDropOp->SrcUINumber);
	if (!Widget)
		return false;
	
	Widget->OnDragDropEvent.Broadcast(DragDropOp);
	
	return true;
}

UDragDropOperation* USYWidgetBase::CreateSrcDragDropOp()
{
	return nullptr;
}

void USYWidgetBase::SetDstDragDropOp(UDragDropOperation* OutDragDropOp)
{

}

USYMouseButtonDownOp* USYWidgetBase::CreateButtonDownOp(FKey key)
{
	return nullptr;
}
