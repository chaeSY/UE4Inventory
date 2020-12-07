// Fill out your copyright notice in the Description page of Project Settings.


#include "SYGameDataManager.h"
#include "SYDataTable.h"

USYGameDataManager::USYGameDataManager()
{
}

USYGameDataManager::~USYGameDataManager()
{
}

FSYTableBase* USYGameDataManager::GetGameData(ETableType TableType, int RowID)
{
	UDataTable* DataTable = TableMap.FindRef(TableType);
	if (DataTable)
	{
		return DataTable->FindRow<FSYTableBase>(*FString::FromInt(RowID), TEXT("GetGameData"));
	}

	return nullptr;
}