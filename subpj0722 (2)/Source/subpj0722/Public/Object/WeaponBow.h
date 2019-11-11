// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/******************************************/
// �÷��̾ ����ִ� Ȱ (���, �ƹ� �ɷ¾���)
/******************************************/


#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponBow.generated.h"

UCLASS()
class SUBPJ0722_API AWeaponBow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Bow;
};
