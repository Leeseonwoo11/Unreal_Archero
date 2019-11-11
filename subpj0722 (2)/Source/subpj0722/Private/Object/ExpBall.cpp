// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpBall.h"
#include "AIController.h"
#include "ABCharacter.h"

// Sets default values
AExpBall::AExpBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(false);

	GetCapsuleComponent()->ComponentTags.Add(FName("ExpBall"));
	GetCapsuleComponent()->SetCollisionProfileName(FName("ExpBall"));
	GetCharacterMovement()->MaxWalkSpeed = 1800.0f; //경험치볼 속도
	AIControllerClass = AAIController::StaticClass(); //경험치볼의 AIController 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 

}

// Called when the game starts or when spawned
void AExpBall::BeginPlay()
{
	Super::BeginPlay();
	Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->NoEnemyInField.AddUFunction(this, "AbsorbInCharacter");
	GetCapsuleComponent()->SetCollisionProfileName(FName("ExpBall"));
}

// Called every frame
void AExpBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExpBall::AbsorbInCharacter() //경험치볼이 캐릭터에 흡수됨이 시작
{
	if (IsActive() != false)
	{
		GetCapsuleComponent()->SetCollisionProfileName(FName("AbsorbExpBall"));
		UE_LOG(LogTemp, Error, TEXT("AbsorbStart"));
		IsAbsorb = true;
		FVector Goal = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetMesh()->GetComponentLocation();
		Cast<AAIController>(GetController())->MoveToLocation(Goal,-1.0f,true,true,false,true,0,true);
	}
	else
	{
		IsAbsorb = false;
	}
}

void AExpBall::SetActive(bool InActive) //활성화 또는 비활성화 셋팅 
{
	Active = InActive;
	SetActorHiddenInGame(!InActive);
}

void AExpBall::Deactivate() //흡수되었을때 디엑티브는 블루프린트에서 처리함
{
	this->SetActive(false);
	SetActorEnableCollision(false);
}

bool AExpBall::IsActive()  //활성화 여부 리턴
{
	return Active;
}