// Fill out your copyright notice in the Description page of Project Settings.


#include "SYUIBase.h"
#include "SYUtil.h"
#include "SYWidgetBase.h"

void USYUIBase::Init(EUINumber InUINumber, USYWidgetBase* InWidget)
{
	UINumber = InUINumber;
	Widget = InWidget;
}