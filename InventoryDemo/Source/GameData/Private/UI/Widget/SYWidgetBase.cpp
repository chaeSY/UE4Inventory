// Fill out your copyright notice in the Description page of Project Settings.


#include "SYWidgetBase.h"
#include "Components/CanvasPanelSlot.h"

void USYWidgetBase::SetZOrder(int ZOrder)
{
	UCanvasPanelSlot* LayoutCanvas = Cast<UCanvasPanelSlot>(Slot);
	if (!LayoutCanvas)
		return;

	LayoutCanvas->SetZOrder(ZOrder);
}
