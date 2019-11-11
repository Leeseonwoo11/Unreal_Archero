// Fill out your copyright notice in the Description page of Project Settings.

//************************************************
// �Ϲݸ� ���� ������ �����ǰ� �÷��̾�� �ο�� ��
// �ʿ��� ������ ������ �����ϰ� �ȴ�.
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
	UStaticMeshComponent* Body; // �� ���
	UPROPERTY(EditAnywhere, Category = "Spawner")
	UObjectPool* ObjectPooler; //������Ʈ Ǯ,(���� ����ġ�� ������ �������)
	UFUNCTION(BlueprintCallable)
	void Spawn(int32 posX = 0, int32 posY = 0); //������
	UFUNCTION(BlueprintCallable)
	void ExpSpawn(); // ����ġ�� ����
	UFUNCTION(BlueprintCallable)
	void BossSpawn(int32 posX = 0, int32 posY = 0); //���������Լ�
	UPROPERTY()
	TArray<AABEnemy*> SpawnedEnemy; //������ ��(Ȱ��ȭ �� ��) ������ Ǯ
	UPROPERTY()
	TArray<AABBoss*> SpawnedBoss; //������ ����(Ȱ��ȭ ����) ������ Ǯ
	UFUNCTION()
	void FindshortestEnmey(); //���� ����� ���� ��ġ ã�� �Լ�;
	FVector ExpSpawnloc; //����ġ�� ���� �����̼�;
};
