// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "ConstructorHelpers.h"
#include "ABCharacter.h"



// Sets default values
AArrow::AArrow()
{
	AArrow::Tags.Add(FName("ARROW"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//화살의 구 콜리젼
	Arrow = CreateDefaultSubobject<USphereComponent>(TEXT("ARROW"));
	Arrow->SetCollisionProfileName(TEXT("Projectile"));
	Arrow->InitSphereRadius(8.5f);
	Arrow->ComponentTags.Add(FName("ARROW"));
	RootComponent = Arrow;

	//화살의 궤적
	Arrow_Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ARROW_TRAIL"));
	Arrow_Trail->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UParticleSystem>PS_ARROW_TRAIL(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBall"));
	if (PS_ARROW_TRAIL.Succeeded())
	{
		UE_LOG(LogType, Error, TEXT("SET ARROW SHAPE"));
		Arrow_Trail->SetTemplate(PS_ARROW_TRAIL.Object);
	}

	ProjectileMovenent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovenent->SetUpdatedComponent(Arrow);
	ProjectileMovenent->InitialSpeed = 3000.0f;
	ProjectileMovenent->MaxSpeed = 3000;
	ProjectileMovenent->Velocity = FVector(0, 0, 0);
	ProjectileMovenent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	Damage = 50;
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AArrow::IsActive()
{
	return Active;
}

void AArrow::SetActive(bool bval)
{
	Active = bval;
	SetActorHiddenInGame(!bval);
}










