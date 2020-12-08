// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SYDataTable.h"
#include "SYGameDataManager.generated.h"

/**
 * 
 */

class UDataTable;
struct FSYTableBase;

UCLASS(Blueprintable)
class GAMEDATA_API USYGameDataManager : public UObject
{
	GENERATED_BODY()

public:
	USYGameDataManager();
	~USYGameDataManager();
	
	FSYTableBase* GetGameData(ETableType TableType, int RowID);

	template<typename T>
	T* GetGameData(ETableType TableType, int RowID)
	{
		return static_cast<T*>(GetGameData(TableType, RowID));
	}

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<ETableType, UDataTable*> TableMap; 
};