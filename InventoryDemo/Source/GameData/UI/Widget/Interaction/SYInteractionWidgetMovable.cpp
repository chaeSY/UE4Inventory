// Fill out your copyright notice in the Description page of Project Settings.


#include "SYInteractionWidgetMovable.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "SYWidgetBase.h"
#include "SYUtil.h"

void USYInteractionWidgetMovable::OnDragging(UDragDropOperation* InOperation)
{
	USYWidgetBase* ParentWidget = SYUtil::GetWidget(GetWorld(), ParentUINumber);
	if (!ParentWidget)
		return;
	
	UCanvasPanelSlot* LayoutCanvas = Cast<UCanvasPanelSlot>(ParentWidget->Slot);
	if (!LayoutCanvas)
		return;
	
	FVector2D alignment = LayoutCanvas->GetAlignment();
	FAnchors  anchor = LayoutCanvas->GetAnchors();
	FVector2D WidgetPos = LayoutCanvas->GetPosition(); // anchor ±âÁØÀÇ ÁÂÇ¥
	FVector2D WidgetSize = LayoutCanvas->GetSize();

	FGeometry vpgeo = UWidgetLayoutLibrary::GetViewportWidgetGeometry(GetWorld());
	FVector2D vpAnchorPos = vpgeo.Size * anchor.Minimum;
	FVector2D vpAlignmentSize = WidgetSize * alignment;

	FVector2D vpWidgetPos = WidgetPos + vpAnchorPos - vpAlignmentSize; //ÇöÀç À§Á¬ÀÇ ½ÇÁ¦ ºäÆ÷Æ® ÁÂÇ¥
	FVector2D Offset = vpWidgetPos - WidgetPos;

	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D ResultPos = MousePos - Offset;
	
	ResultPos -= InOperation->Offset;
	LayoutCanvas->SetPosition(ResultPos);
}