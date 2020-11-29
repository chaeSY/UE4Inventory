// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SYGameInstance.generated.h"

/**
 * 
 */
class USYGameDataManager;
UCLASS()
class GAMEDATA_API USYGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USYGameDataManager* GetGameDataManager();

public:
	virtual void Init() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USYGameDataManager> GameDataManagerClass;

	UPROPERTY()
	USYGameDataManager* GameDataManager;
};
