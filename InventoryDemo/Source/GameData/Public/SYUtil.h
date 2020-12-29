// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/SYGameDataManager.h"
/**
 * 
 */
class ASYCharacter;
class ASYPlayerController;
class USYUIManager;
class USYUIBase;
class USYWidgetBase;
class USYGameDataManager;
struct FSYTableBase;

class SYUtil
{
public:
	static ASYPlayerController* GetPlayerController(const UObject* WorldContextObject);
	static ASYCharacter* GetCharacter(const UObject* WorldContextObject);

	static USYGameDataManager* GetGameDataManager(const UObject* WorldContextObject);
	static FSYTableBase* GetGameData(const UObject* WorldContextObject, ETableType TableType, int ClassID);
	template<typename T>
	static T* GetGameData(const UObject* WorldContextObject, ETableType TableType, int ClassID);

	static USYUIManager* GetUIManager(const UObject* WorldContextObject);
	static USYUIBase* GetUI(const UObject* WorldContextObject, EUINumber UINumber);
	template<typename T>
	static T* GetUI(const UObject* WorldContextObject, EUINumber UINumber);
};

template<typename T>
inline T * SYUtil::GetUI(const UObject * WorldContextObject, EUINumber UINumber)
{
	return Cast<T>(GetUI(WorldContextObject, UINumber));
}

template<typename T>
inline T * SYUtil::GetGameData(const UObject* WorldContextObject, ETableType TableType, int ClassID)
{
	USYGameDataManager* GameDataManager = GetGameDataManager(WorldContextObject);
	if (!GameDataManager)
		return nullptr;

	return GameDataManager->GetGameData<T>(TableType, ClassID);
}
