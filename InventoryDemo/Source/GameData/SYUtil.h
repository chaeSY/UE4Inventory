// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetManager.h"
#include "GameData/SYGameDataManager.h"
/**
 * 
 */
class SYUtil
{
public:
	static class UWidgetManager* GetWidgetManager(const UObject* WorldContextObject);
	static class USYWidgetBase* GetWidget(const UObject* WorldContextObject, EUINumber UINumber);
	template<typename T>
	static T* GetWidget(const UObject* WorldContextObject, EUINumber UINumber);


	static class USYGameDataManager* GetGameDataManager(const UObject* WorldContextObject);
	static struct FSYTableBase* GetGameData(const UObject* WorldContextObject, ETableType TableType, int ClassID);
	template<typename T>
	static T* GetGameData(const UObject* WorldContextObject, ETableType TableType, int ClassID);
};

template<typename T>
inline T * SYUtil::GetWidget(const UObject * WorldContextObject, EUINumber UINumber)
{
	UWidgetManager* WidgetManager = GetWidgetManager(WorldContextObject);
	if (!WidgetManager)
		return nullptr;

	return WidgetManager->GetWidget<T>(UINumber);
}

template<typename T>
inline T * SYUtil::GetGameData(const UObject* WorldContextObject, ETableType TableType, int ClassID)
{
	USYGameDataManager* GameDataManager = GetGameDataManager(WorldContextObject);
	if (!GameDataManager)
		return nullptr;

	return GameDataManager->GetGameData<T>(TableType, ClassID);
}
