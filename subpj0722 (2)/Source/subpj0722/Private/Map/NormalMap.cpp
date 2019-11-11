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

void ANormalMap::Spawn(int32 posX, int32 posY) // �Ϲ� ���� �����ϱ� (Ȱ��ȭ ��)
{
	AABEnemy* PoolableActor = ObjectPooler->GetPooledEnemyObject();
	if (PoolableActor == nullptr)
	{
		return;
	}
	PoolableActor->HP = 200.0f; //���� HP 
	int32 X = posX;
	int32 Y = posY;
	PoolableActor->SetActorLocation(FVector(X, Y, 150));
	PoolableActor->SetActive(true);
	PoolableActor->SetActorEnableCollision(true);
	PoolableActor->SetActorRotation(FRotator().ZeroRotator);
	PoolableActor->GetCharacterMovement()->GravityScale = 1.0f;
	SpawnedEnemy.Push(PoolableActor);
}

void ANormalMap::ExpSpawn()// ����ġ �� �����ϱ�(Ȱ��ȭ ��)
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

void ANormalMap::BossSpawn(int32 posX, int32 posY) //���� �����ϱ�(Ȱ��ȭ����)
{
	AABBoss* PoolableActor = ObjectPooler->GetBossPooledObject();
	if (PoolableActor == nullptr)
	{
		return;
	}
	PoolableActor->BossHP = 50000; // ���� ���뼳��
	PoolableActor->SetActorLocation(FVector(posX, posY, 150));
	PoolableActor->SetActive(true);
	PoolableActor->SetActorEnableCollision(true);
	PoolableActor->SetActorRotation(FRotator().ZeroRotator);
	PoolableActor->GetCharacterMovement()->GravityScale = 1.0f;
	SpawnedBoss.Push(PoolableActor);
	Cast<UABGameInstance>(GetGameInstance())->SetBossPTR(PoolableActor);
}

void ANormalMap::FindshortestEnmey() //���尡��� ���� ��ġ�� ã�� �Լ�
{
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); //�÷��̾�ĳ���� ������;
	if (temp == nullptr)
	{
		return;
	}
	FVector Charloc = temp->GetMesh()->GetComponentLocation();
	FVector MinDistanceActorVector = FVector(10000, 10000, 10000); //���� �ӽð�
	AABEnemy* Target = nullptr;
	for (AABEnemy * Enemys : SpawnedEnemy) //������ ���� �������߿��� ĳ���Ϳ� ���� ����� ��ġ�� ���� ã�´�.
	{
		if (Enemys->IsActive())
		{
			if ((Charloc - Enemys->GetMesh()->GetComponentLocation()).Size() < MinDistanceActorVector.Size())
			{
				MinDistanceActorVector = Enemys->GetMesh()->GetComponentLocation();
				Target = Enemys; //�����Ѵٸ� Ÿ�ٿ� �����ȴ�.
			}
		}
	}
	//�������� �ʴ´ٸ� �ΰ��� Ÿ���̵ȴ�.
	Cast<UABGameInstance>(GetGameInstance())->SetEnemyptr(Target);//Ÿ���� �����ν��Ͻ��� ���� Ÿ������ ���� �����ͷ� ���Ѵ�
}



