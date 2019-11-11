// Fill out your copyright notice in the Description page of Project Settings.


#include "ACEnemy.h"
#include "MacicBall.h"

AACEnemy::AACEnemy()
{
	//스켈레탈 매시 셋
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/ImmortalSK"));
	if (SK_BODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	// 애니메이션 셋
	ConstructorHelpers::FClassFinder<UAnimInstance>ANIM_ACENEMY(TEXT("/Game/Mynew/EnemyAnim/ACEnemyAnimBlueprint"));
	if (ANIM_ACENEMY.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_ACENEMY.Class);
	}
	
}

void AACEnemy::MagicBallFire() //8방으로 매직볼을 날려보낸다.
{
	UWorld* World = GetWorld();

	if (World)
	{
		UE_LOG(LogTemp, Error, TEXT("MagicBallFire!!"));

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AMacicBall* Projectile1 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot-FRotator(0.0,-90.0f,0), SpawnParams);
		AMacicBall* Projectile2 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, -45.0f, 0), SpawnParams);
		AMacicBall* Projectile3 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, 0.0f, 0), SpawnParams);
		AMacicBall* Projectile4 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, 45.0f, 0), SpawnParams);
		AMacicBall* Projectile5 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, 90.0f, 0), SpawnParams);
		AMacicBall* Projectile6 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, 135.0f, 0), SpawnParams);
		AMacicBall* Projectile7 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, 180.0f, 0), SpawnParams);
		AMacicBall* Projectile8 = World->SpawnActor<AMacicBall>(AMacicBall::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, -135.0f, 0), SpawnParams);

	}

}
