// Fill out your copyright notice in the Description page of Project Settings.


#include "SYGameInstance.h"
#include "GameData/SYGameDataManager.h"

USYGameDataManager* USYGameInstance::GetGameDataManager()
{
	return GameDataManager;
}

void USYGameInstance::Init()
{
	if (GameDataManagerClass)
	{
		GameDataManager = NewObject<USYGameDataManager>(this, GameDataManagerClass);
	}
}
