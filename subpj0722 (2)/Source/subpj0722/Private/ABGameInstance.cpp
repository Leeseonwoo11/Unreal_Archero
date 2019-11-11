// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"


void UABGameInstance::Init()
{
	Super::Init();
	TableMgr = TableManager::GetInstance(); //테이블매니져 싱글턴으로 만듬 인스턴스 가져옴;
	InsCurrentMaxExp = TableMgr->GetEXPperLevel(InsCurrentLevel); //최초의 레벨은 1레벨 , 레벨당 경험치를 테이블매니져에서 가져와서 초기화.
	MaxLevel = TableMgr->MaxLevel; //테이블매니져에서 최대레벨 초기화
	UE_LOG(LogTemp, Error, TEXT("Current EXP is %d"), InsCurrentMaxExp);
}

void UABGameInstance::SetCurrentHP(float value) //캐릭터의 현재 HP 세팅
{
	CharCurrentHP = value;
}

void UABGameInstance::SetMaxHP(float value) //캐릭터의 HP 최대치 세팅
{
	CharMaxHp = value;
}
void UABGameInstance::SetCurrentExp(int32 val) //현재 캐릭터의 경험치량
{
	InsCurrentExp = val;
}
void UABGameInstance::SetCurrentMaxExp(int32 CurrentLevel) //현재 캐릭터의 레벨에 따른 캐릭터의 최대경험치량
{
	TableMgr = TableManager::GetInstance();
	InsCurrentMaxExp = TableMgr->GetEXPperLevel(CurrentLevel);

}
void UABGameInstance::SetCurrentLevel(int32 val) //현재 캐릭터의 레벨 세팅
{
	if (val < MaxLevel) //최대레벨보다 작을때
	{
		InsCurrentLevel = val;
	}
	else if (val == MaxLevel) //최대레벨일때
	{
		InsCurrentLevel = val;
		InsCurrentExp = TableMgr->GetEXPperLevel(MaxLevel);
	}
}
int32 UABGameInstance::GetCurrentLevel() //현재 레벨을 리턴한다.
{
	return InsCurrentLevel;
}
void UABGameInstance::SetEnemyptr(AABEnemy* Enemyptr) //타겟팅할 에너미의 포인터를 세팅한다.
{
	if (Enemyptr != nullptr)
	{
		InsEnemy = Enemyptr;
	}
	else //널이라면 널을 대입해준다.
	{
		InsEnemy = Enemyptr;
		//UE_LOG(LogTemp, Error, TEXT("Enemyptr is null ABGameInstance"));
	}
}
void UABGameInstance::SetDeadEnemyptr(AABEnemy * Enemyptr)// 죽은 적의 포인터를 셋팅 (경험치볼스폰하기위함)
{
	InsDeadEnmey = Enemyptr;
}
void UABGameInstance::CharacterCallToken() // 캐릭터클래스에서 불려지는 함수로 실행되게되면 NormalMap에서 가장 가까운적을 찾는 함수를 실행하게됨
{
	FindEnmey_MapCall.ExecuteIfBound();//NormalMap.cpp 에 함수 바인드됨
}

void UABGameInstance::SetbGIDiagonal(bool val) //사선화살 세팅
{
	bGIDiagonal = val;
}

void UABGameInstance::SetbGIMulti(bool val) //멀티샷 세팅
{
	bGIMulti = val;
}

void UABGameInstance::SetbGIRear(bool val) //후방화살 세팅
{
	bGIRear = val;
}

void UABGameInstance::SetbGIRightAndLeft(bool val) //좌우화살세팅
{
	bGIRightAndLeft = val;
}

void UABGameInstance::SetbGIPenetrate(bool val)// 관통화살세팅
{
	bGIPenetrate = val;
}

void UABGameInstance::SpawnExpBallfunc() //경험치볼을 스폰하기위한 함수.
{
	SpawnExpBall.ExecuteIfBound(); //NormalMap.cpp 에 함수 바인드됨
}

void UABGameInstance::SetBossPTR(AABBoss * ptr) //타겟팅할 보스의 포인터 세팅
{
	if (ptr != nullptr)
	{
		if (ptr->IsActive())
		{
			BossPTR = ptr;
		}
		else
		{
			BossPTR = nullptr;
			UE_LOG(LogTemp, Error, TEXT("Bossptr is Not Active ABGameInstance"));
		}
	}
	else
	{
		BossPTR = nullptr;
		UE_LOG(LogTemp, Error, TEXT("Bossptr is null ABGameInstance"));

	}
}

void UABGameInstance::SetDefault()//기본값으로 돌리기
{
	Stage = 1;
	CharCurrentHP = 100000; //캐릭터의 기본 HP
	CharMaxHp = 100000; //캐릭터 기본 최대 HP
	bGIDiagonal = false; //사선화살 기본
	bGIMulti = false;// 멀티샷 기본
	bGIRear = false;// 후방샷 기본
	bGIRightAndLeft = false;//좌우샷 기본
	bGIPenetrate = false;// 관통샷 기본
	InsCurrentLevel = 1; //캐릭 현재레벨 기본
	InsCurrentExp = 0;
	InsCurrentMaxExp = TableMgr->GetEXPperLevel(InsCurrentLevel);//캐릭 최대 경험치량 기본
	MaxLevel = TableMgr->MaxLevel; //최대레벨 기본
}
