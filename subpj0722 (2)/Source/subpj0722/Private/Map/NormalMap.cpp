// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalMap.h"
#include "ABGameInstance.h"
#include "ABCharacter.h"


ANormalMap::ANormalMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GROUND"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>SM_GROUND(TEXT("/Game/Mynew/Maps/SM_MERGED_Pl_16m_Platform_32"));
	if (SM_GROUND.Succeeded())
	{
		Body->SetStaticMesh(SM_GROUND.Object);
	}
	RootComponent = Body;
	ObjectPooler = CreateDefaultSubobject<UObjectPool>(TEXT("ObjectPooler"));
}

void ANormalMap::BeginPlay()
{
	Super::BeginPlay();

	Cast<UABGameInstance>(GetGameInstance())->FindEnmey_MapCall.BindUFunction(this ,"FindshortestEnmey");
	Cast<UABGameInstance>(GetGameInstance())->SpawnExpBall.BindUFunction(this, "ExpSpawn");
}

void ANormalMap::Tick(float DeltaTime)
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

void ANormalMap::Spawn(int32 posX, int32 posY) // 일반 몬스터 스폰하기 (활성화 함)
{
	AABEnemy* PoolableActor = ObjectPooler->GetPooledEnemyObject();
	if (PoolableActor == nullptr)
	{
		return;
	}
	PoolableActor->HP = 200.0f; //적의 HP 
	int32 X = posX;
	int32 Y = posY;
	PoolableActor->SetActorLocation(FVector(X, Y, 150));
	PoolableActor->SetActive(true);
	PoolableActor->SetActorEnableCollision(true);
	PoolableActor->SetActorRotation(FRotator().ZeroRotator);
	PoolableActor->GetCharacterMovement()->GravityScale = 1.0f;
	SpawnedEnemy.Push(PoolableActor);
}

void ANormalMap::ExpSpawn()// 경험치 볼 스폰하기(활성화 함)
{
	ExpSpawnloc = Cast<UABGameInstance>(GetGameInstance())->InsDeadEnmey->GetActorLocation();
	AExpBall* PoolableActor = ObjectPooler->GetExpPooledObject();
	if (PoolableActor == nullptr)
	{
		return;
	}
	PoolableActor->SetActive(true);
	PoolableActor->SetActorEnableCollision(true);
	PoolableActor->GetCharacterMovement()->GravityScale = 1.0f;
	PoolableActor->SetActorLocation(ExpSpawnloc);
}

void ANormalMap::BossSpawn(int32 posX, int32 posY) //보스 스폰하기(활성화까지)
{
	AABBoss* PoolableActor = ObjectPooler->GetBossPooledObject();
	if (PoolableActor == nullptr)
	{
		return;
	}
	PoolableActor->BossHP = 50000; // 보스 피통설정
	PoolableActor->SetActorLocation(FVector(posX, posY, 150));
	PoolableActor->SetActive(true);
	PoolableActor->SetActorEnableCollision(true);
	PoolableActor->SetActorRotation(FRotator().ZeroRotator);
	PoolableActor->GetCharacterMovement()->GravityScale = 1.0f;
	SpawnedBoss.Push(PoolableActor);
	Cast<UABGameInstance>(GetGameInstance())->SetBossPTR(PoolableActor);
}

void ANormalMap::FindshortestEnmey() //가장가까운 적의 위치를 찾는 함수
{
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); //플레이어캐릭터 포인터;
	if (temp == nullptr)
	{
		return;
	}
	FVector Charloc = temp->GetMesh()->GetComponentLocation();
	FVector MinDistanceActorVector = FVector(10000, 10000, 10000); //최초 임시값
	AABEnemy* Target = nullptr;
	for (AABEnemy * Enemys : SpawnedEnemy) //스폰된 적의 포인터중에서 캐릭터와 가장 가까운 위치의 적을 찾는다.
	{
		if (Enemys->IsActive())
		{
			if ((Charloc - Enemys->GetMesh()->GetComponentLocation()).Size() < MinDistanceActorVector.Size())
			{
				MinDistanceActorVector = Enemys->GetMesh()->GetComponentLocation();
				Target = Enemys; //존재한다면 타겟에 설정된다.
			}
		}
	}
	//존재하지 않는다면 널값이 타겟이된다.
	Cast<UABGameInstance>(GetGameInstance())->SetEnemyptr(Target);//타겟을 게임인스턴스의 현재 타겟팅할 적의 포인터로 셋한다
}



