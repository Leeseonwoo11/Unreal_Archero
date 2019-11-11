// Fill out your copyright notice in the Description page of Project Settings.


//**********************************
// ������ƮǮ
// �Ϲ�,��������,����ġ���� ���忡 ��Ȱ��ȭ���� ������ ���� ������ ���� �����͸� Ǯ�� ��Ƴ���
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
	TSubclassOf<class AABEnemy> PooledObjectSubclass1; //�ٰŸ� ���ʹ� bp���� �޴´�
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	TSubclassOf<class AABEnemy> PooledObjectSubclass2; //���Ÿ� ���ʹ� bp���� �޴´�
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	TSubclassOf<class AExpBall> PooledObjectSubclass3; // ����ġ �� bp���� �޴´�
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	TSubclassOf<class AABBoss> PooledObjectSubclass4; //���� bp���� �޴´�

	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	int32 EnemyPoolSize = 10;
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	int32 ExpBallPoolSize = 10;
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	int32 BossPollSize = 3;

	TArray<AABEnemy*> Pool; //���ʹ� Ǯ
	TArray<AExpBall*> ExpPool; //����ġ�� Ǯ
	TArray<AABBoss*> BossPool; //����Ǯ

};
