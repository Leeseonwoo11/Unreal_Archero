// Fill out your copyright notice in the Description page of Project Settings.

//**********************************
//몬스터 없는맵  오브젝트풀을 가지고 있지 않아서 스폰도 안됨
//보너스맵 느낌으로 쉬어가는 맵 Max HP를 올려주거나 CurrentHP를 올려줄 수 있는 기능을 추가할 계획
//**********************************

#pragma once


#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "NoMonsterMap.generated.h"


UCLASS()
class SUBPJ0722_API ANoMonsterMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoMonsterMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		UStaticMeshComponent* Body;
};
