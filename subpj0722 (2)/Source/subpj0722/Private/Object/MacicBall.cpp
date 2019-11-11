// Fill out your copyright notice in the Description page of Project Settings.


#include "MacicBall.h"

// Sets default values
AMacicBall::AMacicBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 매직볼의 구 컴포넌트
	Body = CreateDefaultSubobject<USphereComponent>(TEXT("BODY"));
	Body->SetCollisionProfileName(TEXT("EnemyProjectile"));
	Body->ComponentTags.Add(FName("MagicBall"));
	RootComponent = Body;

	// 매직볼의 파티클 
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE"));
	Particle->SetupAttachment(Body);
	Body->SetWorldScale3D(FVector(2.5f, 2.5f, 2.5f));
	ConstructorHelpers::FObjectFinder<UParticleSystem>EFFECT_BALL(TEXT("/Game/FXVarietyPack/Particles/P_ky_fireStorm"));
	if (EFFECT_BALL.Succeeded())
	{
		Particle->SetTemplate(EFFECT_BALL.Object);
	}
	Particle->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));

	//매직볼의 무브먼트 컴포넌트
	BallMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BALL_MOVEMENTCOMPONENT"));
	BallMovementComponent->SetUpdatedComponent(Body);
	BallMovementComponent->InitialSpeed = 1000.0f;
	BallMovementComponent->MaxSpeed = 1000.0f;
	BallMovementComponent->bShouldBounce = false;
	BallMovementComponent->ProjectileGravityScale = 0;
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AMacicBall::BeginPlay()
{
	Super::BeginPlay();
	Body->OnComponentBeginOverlap.AddDynamic(this, &AMacicBall::OnActorBeginOverlap);
	
}

// Called every frame
void AMacicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMacicBall::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("PLAYER"))) //플레이어와 닿았을때 없애줌
	{
		Destroy();
		UE_LOG(LogTemp, Error, TEXT("MagicBall is Overlapped by PLAYER"));
	}
}

