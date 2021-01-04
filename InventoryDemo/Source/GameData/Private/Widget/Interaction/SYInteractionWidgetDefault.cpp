// Fill out your copyright notice in the Description page of Project Settings.


#include "SYInteractionWidgetDefault.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "SYUtil.h"
#include "SYWidgetBase.h"

UDragDropPayloadBase* USYInteractionWidgetDefault::CreatePayload()
{
	UDragDropPayloadBase* Payload = NewObject<UDragDropPayloadBase>();
	if (Payload)
	{
		Payload->SrcUINumber = ParentUINumber;
	}

	return Payload;
}

void USYInteractionWidgetDefault::SetPayloadOnDrop(UDragDropPayloadBase* Payload)
{
	if (Payload)
	{
		Payload->DstUINumber = ParentUINumber;
	}
}
