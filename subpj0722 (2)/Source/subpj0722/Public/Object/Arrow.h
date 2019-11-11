// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Arrow.generated.h"


//***************************
//플레이어가 발사하는 화살
//***************************

UCLASS()
class SUBPJ0722_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USphereComponent* Arrow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystemComponent* Arrow_Trail;
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovenent;

	UPROPERTY(VisibleAnywhere)
	int32 Damage;
	UPROPERTY(EditAnywhere)
	bool Active = false;
	UFUNCTION()
	bool IsActive();
	UFUNCTION(BlueprintCallable)
	void SetActive(bool bval);
};