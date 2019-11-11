// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameInstance.h"


void UABGameInstance::Init()
{
	Super::Init();
	TableMgr = TableManager::GetInstance(); //���̺�Ŵ��� �̱������� ���� �ν��Ͻ� ������;
	InsCurrentMaxExp = TableMgr->GetEXPperLevel(InsCurrentLevel); //������ ������ 1���� , ������ ����ġ�� ���̺�Ŵ������� �����ͼ� �ʱ�ȭ.
	MaxLevel = TableMgr->MaxLevel; //���̺�Ŵ������� �ִ뷹�� �ʱ�ȭ
	UE_LOG(LogTemp, Error, TEXT("Current EXP is %d"), InsCurrentMaxExp);
}

void UABGameInstance::SetCurrentHP(float value) //ĳ������ ���� HP ����
{
	CharCurrentHP = value;
}

void UABGameInstance::SetMaxHP(float value) //ĳ������ HP �ִ�ġ ����
{
	CharMaxHp = value;
}
void UABGameInstance::SetCurrentExp(int32 val) //���� ĳ������ ����ġ��
{
	InsCurrentExp = val;
}
void UABGameInstance::SetCurrentMaxExp(int32 CurrentLevel) //���� ĳ������ ������ ���� ĳ������ �ִ����ġ��
{
	TableMgr = TableManager::GetInstance();
	InsCurrentMaxExp = TableMgr->GetEXPperLevel(CurrentLevel);

}
void UABGameInstance::SetCurrentLevel(int32 val) //���� ĳ������ ���� ����
{
	if (val < MaxLevel) //�ִ뷹������ ������
	{
		InsCurrentLevel = val;
	}
	else if (val == MaxLevel) //�ִ뷹���϶�
	{
		InsCurrentLevel = val;
		InsCurrentExp = TableMgr->GetEXPperLevel(MaxLevel);
	}
}
int32 UABGameInstance::GetCurrentLevel() //���� ������ �����Ѵ�.
{
	return InsCurrentLevel;
}
void UABGameInstance::SetEnemyptr(AABEnemy* Enemyptr) //Ÿ������ ���ʹ��� �����͸� �����Ѵ�.
{
	if (Enemyptr != nullptr)
	{
		InsEnemy = Enemyptr;
	}
	else //���̶�� ���� �������ش�.
	{
		InsEnemy = Enemyptr;
		//UE_LOG(LogTemp, Error, TEXT("Enemyptr is null ABGameInstance"));
	}
}
void UABGameInstance::SetDeadEnemyptr(AABEnemy * Enemyptr)// ���� ���� �����͸� ���� (����ġ�������ϱ�����)
{
	InsDeadEnmey = Enemyptr;
}
void UABGameInstance::CharacterCallToken() // ĳ����Ŭ�������� �ҷ����� �Լ��� ����ǰԵǸ� NormalMap���� ���� ��������� ã�� �Լ��� �����ϰԵ�
{
	FindEnmey_MapCall.ExecuteIfBound();//NormalMap.cpp �� �Լ� ���ε��
}

void UABGameInstance::SetbGIDiagonal(bool val) //�缱ȭ�� ����
{
	bGIDiagonal = val;
}

void UABGameInstance::SetbGIMulti(bool val) //��Ƽ�� ����
{
	bGIMulti = val;
}

void UABGameInstance::SetbGIRear(bool val) //�Ĺ�ȭ�� ����
{
	bGIRear = val;
}

void UABGameInstance::SetbGIRightAndLeft(bool val) //�¿�ȭ�켼��
{
	bGIRightAndLeft = val;
}

void UABGameInstance::SetbGIPenetrate(bool val)// ����ȭ�켼��
{
	bGIPenetrate = val;
}

void UABGameInstance::SpawnExpBallfunc() //����ġ���� �����ϱ����� �Լ�.
{
	SpawnExpBall.ExecuteIfBound(); //NormalMap.cpp �� �Լ� ���ε��
}

void UABGameInstance::SetBossPTR(AABBoss * ptr) //Ÿ������ ������ ������ ����
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

void UABGameInstance::SetDefault()//�⺻������ ������
{
	Stage = 1;
	CharCurrentHP = 100000; //ĳ������ �⺻ HP
	CharMaxHp = 100000; //ĳ���� �⺻ �ִ� HP
	bGIDiagonal = false; //�缱ȭ�� �⺻
	bGIMulti = false;// ��Ƽ�� �⺻
	bGIRear = false;// �Ĺ漦 �⺻
	bGIRightAndLeft = false;//�¿켦 �⺻
	bGIPenetrate = false;// ���뼦 �⺻
	InsCurrentLevel = 1; //ĳ�� ���緹�� �⺻
	InsCurrentExp = 0;
	InsCurrentMaxExp = TableMgr->GetEXPperLevel(InsCurrentLevel);//ĳ�� �ִ� ����ġ�� �⺻
	MaxLevel = TableMgr->MaxLevel; //�ִ뷹�� �⺻
}
