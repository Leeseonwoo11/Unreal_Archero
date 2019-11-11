// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBow.h"
#include "ConstructorHelpers.h"



// Sets default values
AWeaponBow::AWeaponBow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Bow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerWeaponBow"));
	RootComponent = Bow;
	Bow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BOW(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Bow01SM"));
	if (SM_BOW.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("SET_BOW"));
		Bow->SetStaticMesh(SM_BOW.Object);
	}

}

// Called when the game starts or when spawned
void AWeaponBow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

