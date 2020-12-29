// Fill out your copyright notice in the Description page of Project Settings.


#include "SYUtil.h"
#include "Kismet/GameplayStatics.h"
#include "SYPlayerController.h"
#include "SYGameInstance.h"
#include "SYCharacter.h"
#include "SYUIBase.h"
#include "SYUIManager.h"

ASYPlayerController* SYUtil::GetPlayerController(const UObject* WorldContextObject)
{
	USYGameInstance* GameInstance = Cast<USYGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GameInstance)
		return nullptr;

	ASYPlayerController* PlayerController = Cast<ASYPlayerController>(GameInstance->GetFirstLocalPlayerController());
	return PlayerController;
}

ASYCharacter* SYUtil::GetCharacter(const UObject* WorldContextObject)
{
	ASYPlayerController* Controller = GetPlayerController(WorldContextObject);
	if (!Controller)
		return nullptr;

	ASYCharacter* Character = Cast<ASYCharacter>(Controller->GetCharacter());
	return Character;
}

USYGameDataManager* SYUtil::GetGameDataManager(const UObject * WorldContextObject)
{
	USYGameInstance* GameInstance = Cast<USYGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GameInstance)
		return nullptr;

	return GameInstance->GetGameDataManager();
}

FSYTableBase * SYUtil::GetGameData(const UObject * WorldContextObject, ETableType TableType, int ClassID)
{
	USYGameDataManager* GameDataManager = GetGameDataManager(WorldContextObject);
	if (!GameDataManager)
		return nullptr;

	return GameDataManager->GetGameData(TableType, ClassID);
}

USYUIManager* SYUtil::GetUIManager(const UObject* WorldContextObject)
{
	ASYPlayerController* PC = GetPlayerController(WorldContextObject);
	if (PC)
	{
		return PC->GetUIManager();
	}

	return nullptr;
}

USYUIBase* SYUtil::GetUI(const UObject* WorldContextObject, EUINumber UINumber)
{
	USYUIManager* UIManager = GetUIManager(WorldContextObject);
	if (UIManager)
	{
		return UIManager->GetUI(UINumber);
	}

	return nullptr;
}