// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "MacicBall.generated.h"

//*******************************
// 원거리 몬스터가 던지는 매직 볼
//*******************************

UCLASS()
class SUBPJ0722_API AMacicBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMacicBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY()
		USphereComponent* Body;
	UPROPERTY()
		UParticleSystemComponent* Particle;
	UPROPERTY()
		UProjectileMovementComponent* BallMovementComponent;
	UFUNCTION()
		void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
