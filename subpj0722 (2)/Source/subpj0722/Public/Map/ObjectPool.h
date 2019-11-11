// Fill out your copyright notice in the Description page of Project Settings.


//**********************************
// 오브젝트풀
// 일반,보스몬스터,경험치볼을 월드에 비활성화시켜 스폰해 놓고 스폰한 놈의 포인터를 풀에 담아놓음
//**********************************

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "ABEnemy.h"
#include "ExpBall.h"
#include "ABBoss.h"
#include "Arrow.h"
#include "ObjectPool.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SUBPJ0722_API UObjectPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPool();

	AABEnemy* GetPooledEnemyObject();
	AExpBall* GetExpPooledObject();
	AABBoss* GetBossPooledObject();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	TSubclassOf<class AABEnemy> PooledObjectSubclass1; //근거리 에너미 bp에서 받는다
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	TSubclassOf<class AABEnemy> PooledObjectSubclass2; //원거리 에너미 bp에서 받는다
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	TSubclassOf<class AExpBall> PooledObjectSubclass3; // 경험치 볼 bp에서 받는다
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	TSubclassOf<class AABBoss> PooledObjectSubclass4; //보스 bp에서 받는다

	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	int32 EnemyPoolSize = 10;
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	int32 ExpBallPoolSize = 10;
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	int32 BossPollSize = 3;

	TArray<AABEnemy*> Pool; //에너미 풀
	TArray<AExpBall*> ExpPool; //경험치볼 풀
	TArray<AABBoss*> BossPool; //보스풀

};
