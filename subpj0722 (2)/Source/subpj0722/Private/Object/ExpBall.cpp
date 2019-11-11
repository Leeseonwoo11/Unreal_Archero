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
	GetCharacterMovement()->MaxWalkSpeed = 1800.0f; //����ġ�� �ӵ�
	AIControllerClass = AAIController::StaticClass(); //����ġ���� AIController 
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

void AExpBall::AbsorbInCharacter() //����ġ���� ĳ���Ϳ� ������� ����
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

void AExpBall::SetActive(bool InActive) //Ȱ��ȭ �Ǵ� ��Ȱ��ȭ ���� 
{
	Active = InActive;
	SetActorHiddenInGame(!InActive);
}

void AExpBall::Deactivate() //����Ǿ����� ��Ƽ��� �������Ʈ���� ó����
{
	this->SetActive(false);
	SetActorEnableCollision(false);
}

bool AExpBall::IsActive()  //Ȱ��ȭ ���� ����
{
	return Active;
}