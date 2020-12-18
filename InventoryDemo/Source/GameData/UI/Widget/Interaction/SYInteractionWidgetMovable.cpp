// Fill out your copyright notice in the Description page of Project Settings.


#include "SYInteractionWidgetMovable.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "SYWidgetBase.h"
#include "SYUtil.h"

//UObject* USYInteractionWidgetMovable::CreatePayload()
//{
//
//}
//
//void USYInteractionWidgetMovable::SetPayloadOnDrop(UObject* Payload)
//{
//
//}

//void USYInteractionWidgetMovable::OnDragDrop(UDragDropOperation* InOperation)
//{
//
//}

void USYInteractionWidgetMovable::OnDragging(UDragDropOperation* InOperation)
{
	USYWidgetBase* ParentWidget = SYUtil::GetWidget(GetWorld(), ParentUINumber);
	if (!ParentWidget)
		return;
	
	UCanvasPanelSlot* LayoutCanvas = Cast<UCanvasPanelSlot>(ParentWidget->Slot);
	if (!LayoutCanvas)
		return;
	
	//FGeometry vpgeo = UWidgetLayoutLibrary::GetViewportWidgetGeometry(GetWorld());
	//
	//FVector2D parentWidgetPos = LayoutCanvas->GetPosition(); // anchor ±âÁØÀÇ ÁÂÇ¥
	//FAnchors anchor = LayoutCanvas->GetAnchors();
	//FVector2D anchorPos = vpgeo.Size * anchor.Minimum;
	//
	//FVector2D vpPos = parentWidgetPos + anchorPos; //ÇöÀç À§Á¬ÀÇ ½ÇÁ¦ ºäÆ÷Æ® ÁÂÇ¥
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	//
	//FVector2D Offset = vpPos - parentWidgetPos;
	//FVector2D ResultPos = MousePos - Offset;
	//
	////calc my offset
	//ResultPos -= InOperation->Offset;

	//LayoutCanvas->SetPosition(ResultPos);

	LayoutCanvas->SetPosition(MousePos);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, MousePos.ToString());
}