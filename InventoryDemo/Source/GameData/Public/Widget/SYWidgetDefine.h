// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Coreminimal.h"

DECLARE_EVENT(UUserWidget, FButtonEvent);
DECLARE_EVENT_OneParam(UUserWidget, FSlotEvent, int32 /* SlotIndex */);
DECLARE_EVENT_OneParam(UUserWidget, FTabEvent, int32 /* TabIndex */);
DECLARE_EVENT_FourParams(UUserWidget, FSlotDragDropEvent, EUINumber /* SrcUINumber */, int32 /* SrcSlotIndex */, EUINumber /* DstUINumber */, int32 /* DstSlotIndex */);
