// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "LevelDataTables.h"
#include "UObject/ConstructorHelpers.h"


/**
 *  테이블매니져 싱글턴으로 만들어서 접근을 쉽게 만듬
 */
class SUBPJ0722_API TableManager
{
private:
	TableManager() {};
	~TableManager() {};

	static TableManager* Instance;



public:
	static TableManager* GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = new TableManager();

			Instance->SetLevelDataTable(TEXT("/Game/Mynew/ExpBall/subpj0722_LevelTable")); 
			return Instance;
		}
		else
		{
			return Instance;
		}
	}
	static void ReleaseInstance()
	{
		delete Instance;
		Instance = nullptr;
	}
private:
	class UDataTable* LevelDataTable;

public:
	int32 MaxLevel = 6;
	int32 MinLevel = 1;
	int32 GetEXPperLevel(int32 LV) //레벨당 경험치를 리턴한다. 최소레벨 최대레벨 사이의 값에 레벨이 들어올때만 리턴하고 나머지는 0을리턴
	{
		if (LevelDataTable != nullptr)
		{
			if (LV >= MinLevel && LV <= MaxLevel)
			{
				FLevelDataTableRow* LevelTableRow = LevelDataTable->FindRow<FLevelDataTableRow>(FName(*(FString::FormatAsNumber(LV))), FString(""));
				return LevelTableRow->nMaxExp;
			}
			else
				return 0;
		}
		else
			return 0;
	}

	void SetLevelDataTable(const TCHAR* ContentPath) //캐릭터 레벨 데이터 테이블 셋
	{
		UDataTable* LevelDT = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, ContentPath));
		if (LevelDT)
		{
			LevelDataTable = LevelDT;
		}
	}

};

TableManager* TableManager::Instance = nullptr;


