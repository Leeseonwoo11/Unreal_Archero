// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "Arrow.h"
#include "ArrowPool.generated.h"

// This class does not need to be modified.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SUBPJ0722_API UArrowPool : public UActorComponent
{
	GENERATED_BODY()
public:
	UArrowPool();
	AArrow* GetArrowPooledObject();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "ObjectPooler")
	int32 ArrowPoolSize = 100;
	TArray<AArrow*> ArrowPool;
	TSubclassOf<class AArrow> PooledObjectSubclass;
};

