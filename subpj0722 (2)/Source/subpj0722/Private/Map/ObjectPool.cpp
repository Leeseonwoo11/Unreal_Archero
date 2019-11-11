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


AABEnemy* UObjectPool::GetPooledEnemyObject() //미리생성한 에너미 풀에서 활성화 안된 에너미의 포인터를 리턴
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

AExpBall* UObjectPool::GetExpPooledObject() //미리 생성한 경험치볼 풀에서 활성화가 안된 경험치볼의 포인터를 리턴
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

AABBoss * UObjectPool::GetBossPooledObject() // 미리생성한 보스 풀에서 활성화가 안된 보스의 포인터를 리턴
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
				AABEnemy* PoolableActor1 = GetWorld()->SpawnActor<AABEnemy>(PooledObjectSubclass1, FVector(0, 0, 250.0f), FRotator().ZeroRotator); //풀사이즈만큼 에너미(근거리)스폰
				PoolableActor1->SetActive(false);																								//비활성화
				PoolableActor1->GetCharacterMovement()->GravityScale = 0.0f;																	//중력 0
				PoolableActor1->HP = 0;																											//hp 0;
				Pool.Add(PoolableActor1);																										//에너미 풀에다가 스폰한거 포인터 담아둠

				AABEnemy* PoolableActor2 = GetWorld()->SpawnActor<AABEnemy>(PooledObjectSubclass2, FVector(0, 0, 250.0f), FRotator().ZeroRotator);// 풀사이즈만큼 에너미(원거리)스폰
				PoolableActor2->SetActive(false);																								// 비활성화
				PoolableActor2->GetCharacterMovement()->GravityScale = 0.0f;																	// 중력 0;
				PoolableActor2->HP = 0;																											// hp 0
				Pool.Add(PoolableActor2);																										// 에너미 풀에 스폰한거 포인터 담아둠
			}
			for (int32 i = 0; i < ExpBallPoolSize; i++)
			{
				AExpBall* PoolableActor3 = GetWorld()->SpawnActor<AExpBall>(PooledObjectSubclass3, FVector(0, 0, 250.0f), FRotator().ZeroRotator);		//풀사이즈만큼 경험치볼 스폰
				PoolableActor3->SetActive(false);																										//비활성화
				PoolableActor3->GetCharacterMovement()->GravityScale = 0.0f;																			//중력 0
				ExpPool.Add(PoolableActor3);																											// 경험치볼 풀에 스폰한 경험치볼 포인터 담아둠
			}
			for (int32 i = 0; i < BossPollSize; i++)
			{
				AABBoss* PoolableActor4 = GetWorld()->SpawnActor<AABBoss>(PooledObjectSubclass4, FVector(0, 0, 250.0f), FRotator().ZeroRotator); // 풀사이즈만큼 보스스폰
				PoolableActor4->SetActive(false);																										//보스 비활성화
				PoolableActor4->GetCharacterMovement()->GravityScale = 0.0f;																				//중력 0;
				BossPool.Add(PoolableActor4);																												// 스폰한 보스 보스풀에 포인터 담아둠
			}

		}

	}
}


// Called every frame
void UObjectPool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

