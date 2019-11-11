// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowPool.h"

// Add default functionality here for any IArrowPool functions that are not pure virtual.

UArrowPool::UArrowPool()
{
	ConstructorHelpers::FClassFinder<AArrow>AC_ARROW(TEXT("/Script/subpj0722.Arrow"));
	if (AC_ARROW.Succeeded())
	{
		PooledObjectSubclass = AC_ARROW.Class;
	}
}

AArrow* UArrowPool::GetArrowPooledObject()
{
	for (AArrow* PoolableActor : ArrowPool)
	{
		if (!PoolableActor->IsActive())
		{
			return PoolableActor;
		}
	}
	return nullptr;
}

void UArrowPool::BeginPlay()
{
	
	UWorld* const World = GetWorld();

	if (World)
	{
		for (int32 i = 0; i < ArrowPoolSize; i++)
		{
			AArrow* PoolableActor = GetWorld()->SpawnActor<AArrow>(PooledObjectSubclass, FVector(0, 0, 250.0f), FRotator(0,0,0
			));
			PoolableActor->SetActive(false);
			ArrowPool.Add(PoolableActor);
		}
	}
}
