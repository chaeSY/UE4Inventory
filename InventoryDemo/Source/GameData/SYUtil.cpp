// Fill out your copyright notice in the Description page of Project Settings.


#include "SYUtil.h"
#include "Kismet/GameplayStatics.h"
#include "SYPlayerController.h"
#include "SYGameInstance.h"


UWidgetManager* SYUtil::GetWidgetManager(const UObject * WorldContextObject)
{
	USYGameInstance* GameInstance = Cast<USYGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GameInstance)
		return nullptr;

	ASYPlayerController* PlayerController = Cast<ASYPlayerController>(GameInstance->GetFirstLocalPlayerController());
	if (!PlayerController)
		return nullptr;
	
	return PlayerController->GetWidgetManager();
}

USYWidgetBase* SYUtil::GetWidget(const UObject* WorldContextObject, EUINumber UINumber)
{
	UWidgetManager* WidgetManager = GetWidgetManager(WorldContextObject);
	if (!WidgetManager)
		return nullptr;

	return WidgetManager->GetWidget(UINumber);
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
