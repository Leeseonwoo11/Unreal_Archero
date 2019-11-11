// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"

// Sets default values for this component's properties
UObjectPool::UObjectPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


AABEnemy* UObjectPool::GetPooledEnemyObject() //�̸������� ���ʹ� Ǯ���� Ȱ��ȭ �ȵ� ���ʹ��� �����͸� ����
{
	for (AABEnemy* PoolableActor : Pool)
	{
		if (!PoolableActor->IsActive())
		{ 
			return PoolableActor;
		}
	}
	return nullptr;
}

AExpBall* UObjectPool::GetExpPooledObject() //�̸� ������ ����ġ�� Ǯ���� Ȱ��ȭ�� �ȵ� ����ġ���� �����͸� ����
{
	for (AExpBall* PoolableActor : ExpPool)
	{
		if (!PoolableActor->IsActive())
		{
			return PoolableActor;
		}
	}
	return nullptr;
}

AABBoss * UObjectPool::GetBossPooledObject() // �̸������� ���� Ǯ���� Ȱ��ȭ�� �ȵ� ������ �����͸� ����
{
	for (AABBoss* PoolableActor : BossPool)
	{
		if (!PoolableActor->IsActive())
		{
			return PoolableActor;
		}
	}
	return nullptr;
}



// Called when the game starts
void UObjectPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectSubclass1 != NULL && PooledObjectSubclass2 != NULL && PooledObjectSubclass3 != NULL && PooledObjectSubclass4 != NULL)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			for (int32 i = 0; i < EnemyPoolSize; i++)
			{
				AABEnemy* PoolableActor1 = GetWorld()->SpawnActor<AABEnemy>(PooledObjectSubclass1, FVector(0, 0, 250.0f), FRotator().ZeroRotator); //Ǯ�����ŭ ���ʹ�(�ٰŸ�)����
				PoolableActor1->SetActive(false);																								//��Ȱ��ȭ
				PoolableActor1->GetCharacterMovement()->GravityScale = 0.0f;																	//�߷� 0
				PoolableActor1->HP = 0;																											//hp 0;
				Pool.Add(PoolableActor1);																										//���ʹ� Ǯ���ٰ� �����Ѱ� ������ ��Ƶ�

				AABEnemy* PoolableActor2 = GetWorld()->SpawnActor<AABEnemy>(PooledObjectSubclass2, FVector(0, 0, 250.0f), FRotator().ZeroRotator);// Ǯ�����ŭ ���ʹ�(���Ÿ�)����
				PoolableActor2->SetActive(false);																								// ��Ȱ��ȭ
				PoolableActor2->GetCharacterMovement()->GravityScale = 0.0f;																	// �߷� 0;
				PoolableActor2->HP = 0;																											// hp 0
				Pool.Add(PoolableActor2);																										// ���ʹ� Ǯ�� �����Ѱ� ������ ��Ƶ�
			}
			for (int32 i = 0; i < ExpBallPoolSize; i++)
			{
				AExpBall* PoolableActor3 = GetWorld()->SpawnActor<AExpBall>(PooledObjectSubclass3, FVector(0, 0, 250.0f), FRotator().ZeroRotator);		//Ǯ�����ŭ ����ġ�� ����
				PoolableActor3->SetActive(false);																										//��Ȱ��ȭ
				PoolableActor3->GetCharacterMovement()->GravityScale = 0.0f;																			//�߷� 0
				ExpPool.Add(PoolableActor3);																											// ����ġ�� Ǯ�� ������ ����ġ�� ������ ��Ƶ�
			}
			for (int32 i = 0; i < BossPollSize; i++)
			{
				AABBoss* PoolableActor4 = GetWorld()->SpawnActor<AABBoss>(PooledObjectSubclass4, FVector(0, 0, 250.0f), FRotator().ZeroRotator); // Ǯ�����ŭ ��������
				PoolableActor4->SetActive(false);																										//���� ��Ȱ��ȭ
				PoolableActor4->GetCharacterMovement()->GravityScale = 0.0f;																				//�߷� 0;
				BossPool.Add(PoolableActor4);																												// ������ ���� ����Ǯ�� ������ ��Ƶ�
			}

		}

	}
}


// Called every frame
void UObjectPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

