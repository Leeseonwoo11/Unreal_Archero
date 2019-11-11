// Fill out your copyright notice in the Description page of Project Settings.


#include "NoMonsterMap.h"
#include "ABCharacter.h"

// Sets default values
ANoMonsterMap::ANoMonsterMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>SM_MAP(TEXT("/Game/Mynew/Maps/SM_MERGED_Pl_16m_Platform_32"));
	if (SM_MAP.Succeeded())
	{
		Body->SetStaticMesh(SM_MAP.Object);
	}

}

// Called when the game starts or when spawned
void ANoMonsterMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANoMonsterMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (temp != nullptr)
	{
		if (temp->CurrentHP < 0)
		{
			UGameplayStatics::OpenLevel(GetWorld(), "DeadMap");
		}
	}
}

