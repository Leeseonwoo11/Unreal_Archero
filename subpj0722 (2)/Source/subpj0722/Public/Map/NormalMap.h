// Fill out your copyright notice in the Description page of Project Settings.

//************************************************
// 일반맵 몬스터 보스가 스폰되고 플레이어와 싸우는 맵
// 맵에서 몬스터의 스폰을 관리하게 된다.
//************************************************

#pragma once

#include "ObjectPool.h"
#include "ABEnemy.h"
#include "ABBoss.h"
#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "NormalMap.generated.h"


UCLASS()
class SUBPJ0722_API ANormalMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANormalMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UStaticMeshComponent* Body; // 맵 모양
	UPROPERTY(EditAnywhere, Category = "Spawner")
	UObjectPool* ObjectPooler; //오브젝트 풀,(몬스터 경험치볼 보스를 담고있음)
	UFUNCTION(BlueprintCallable)
	void Spawn(int32 posX = 0, int32 posY = 0); //적스폰
	UFUNCTION(BlueprintCallable)
	void ExpSpawn(); // 경험치볼 스폰
	UFUNCTION(BlueprintCallable)
	void BossSpawn(int32 posX = 0, int32 posY = 0); //보스스폰함수
	UPROPERTY()
	TArray<AABEnemy*> SpawnedEnemy; //스폰된 적(활성화 된 적) 포인터 풀
	UPROPERTY()
	TArray<AABBoss*> SpawnedBoss; //스폰된 보스(활성화 보스) 포인터 풀
	UFUNCTION()
	void FindshortestEnmey(); //가장 가까운 적의 위치 찾기 함수;
	FVector ExpSpawnloc; //경험치볼 스폰 로케이션;
};
