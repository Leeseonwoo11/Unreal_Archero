// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "LevelDataTables.h"
#include "UObject/ConstructorHelpers.h"


/**
 *  ���̺�Ŵ��� �̱������� ���� ������ ���� ����
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
	int32 GetEXPperLevel(int32 LV) //������ ����ġ�� �����Ѵ�. �ּҷ��� �ִ뷹�� ������ ���� ������ ���ö��� �����ϰ� �������� 0������
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

	void SetLevelDataTable(const TCHAR* ContentPath) //ĳ���� ���� ������ ���̺� ��
	{
		UDataTable* LevelDT = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, ContentPath));
		if (LevelDT)
		{
			LevelDataTable = LevelDT;
		}
	}

};

TableManager* TableManager::Instance = nullptr;


