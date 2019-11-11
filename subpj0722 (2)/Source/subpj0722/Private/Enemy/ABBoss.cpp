// Fill out your copyright notice in the Description page of Project Settings.


#include "ABBoss.h"
#include "ABBossAIController.h"
#include "ABGameInstance.h"
#include "ABCharacter.h"
#include "NavigationSystem.h"
#include "SwordFlame.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AABBoss::AABBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetActorEnableCollision(false); //������ƮǮ�� �����Ҷ� ��Ȱ��ȭ �����̱������� �ݸ����� false�� �ʱ�ȭ ����

	// ���̷�Ż �޽� ȸ��,��ġ,�±� ��
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	GetMesh()->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	GetMesh()->SetCollisionProfileName(FName("EnemyBody"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/ExecutionerSK"));
	if (SK_BODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	// �ִϸ��̼� ��
	ConstructorHelpers::FClassFinder<UAnimInstance>ANIM_ABBOSS(TEXT("/Game/Mynew/EnemyAnim/ABBossAnimBlueprint"));
	if (ANIM_ABBOSS.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_ABBOSS.Class);
	}
	//AI��Ʈ�ѷ� �� (�⺻ AIController)
	AIControllerClass = AABBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// ��Ʈ�ѷ� ����
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 480.0f, 0);
}

// Called when the game starts or when spawned
void AABBoss::BeginPlay()
{
	Super::BeginPlay();
	
	//ĸ��������Ʈ ������ �Լ� ���ε�
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AABBoss::OnActorBeginOverlap);
}


// Called every frame
void AABBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->IsActive()) //Ȱ��ȭ ���¶�� ĳ���͸� ó�ٺ���. 
	{
		LookCharacter(); 
	}
	if (BossHP <= 0) //�ǰ� 0���� �۾�����
	{
		IsDead = true; 
		Deactivate();
	}
}

void AABBoss::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if(!IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f; // ���� ���� �ȱ� �ӵ�
	}
}

void AABBoss::LookCharacter() // �÷��̾� ó�ٺ���
{
	FVector Enemyloc = GetMesh()->GetComponentLocation();
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (temp != nullptr)
	{
		FVector Characterloc = temp->GetMesh()->GetComponentLocation();
		FRotator CharacterRot = UKismetMathLibrary::FindLookAtRotation(Enemyloc, Characterloc) + FRotator(0, -90, 0);
		GetMesh()->SetWorldRotation(CharacterRot);
	}
}

void AABBoss::RushAttack() //���ݰ���
{
	if (this->IsActive())
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.0f; // �ӵ� 3�� �÷���
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (NavSystem == nullptr)
		{
			return;
		}
		FNavLocation NextLocation;
		AABCharacter* ABCharacter = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (ABCharacter == nullptr)
		{
			return;
		}
		NextLocation.Location = ABCharacter->GetActorLocation();
		Cast<AABBossAIController>(GetController())->MoveToLocation(NextLocation);
		IsRush = true;
		FTimerHandle RushAttackTimer;
		GetWorldTimerManager().SetTimer(RushAttackTimer, this, &AABBoss::RushAttackEnd, 0.7, false, 1.0f);
	}
}

void AABBoss::RushAttackEnd() //���� ���� ����
{
	if (this->IsActive())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f; //�ӵ� �ٽ� �������
		IsRush = false;
		OnRushAttackEnd.Broadcast(); // BossAI�� BTTaskNode_RushAttack.cpp(26)�� ���ε� �Ǿ����� (�����̺�� Ʈ��)
	}
}

void AABBoss::MeleeAttack() //��������
{
	if (this->IsActive())
	{
		IsMelee = true;
		FTimerHandle MeleeAttackTimer;
		GetWorldTimerManager().SetTimer(MeleeAttackTimer, this, &AABBoss::MeleeAttackEnd, 5.0, false, 1.0f);
	}
}

void AABBoss::MeleeAttackEnd() //�������� ����
{
	if (this->IsActive())
	{
		IsMelee = false;
		OnMeleeAttackEnd.Broadcast(); //BossAI�� BTTaskNodeMelee.cpp(25)�� ���ε� �Ǿ�����(�����̺�� Ʈ��)
	}
}

void AABBoss::FlameAttack() //���Ÿ� �˱����
{
	if (this->IsActive())
	{
		IsFlame = true;
		AABCharacter* ABCharacter = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); //�÷��̾��� ������
		if (ABCharacter == nullptr)
		{
			return;
		}
		UWorld* World = GetWorld();

		if (World) //�����Ѵ�.(ĳ���Ϳ� �߻��Ѵ�.)
		{
			UE_LOG(LogTemp, Error, TEXT("MagicBallFire!!"));
			FVector Enemyloc = GetMesh()->GetComponentLocation();
			FVector Characterloc = ABCharacter->GetActorLocation();
			FRotator CharacterRot = UKismetMathLibrary::FindLookAtRotation(Enemyloc, Characterloc) + FRotator(0, -90, 0);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			ASwordFlame* Projectile1 = World->SpawnActor<ASwordFlame>(ASwordFlame::StaticClass(), GetMesh()->GetComponentLocation() + FVector(0, 0, 90.0f), CharacterRot - FRotator(0.0, -90.0f, 0), SpawnParams);
		}
		IsFlame = false;
	}
}

void AABBoss::SetActive(bool IsActive)//Ȱ�� �Ǵ� ��Ȱ��
{
	Active = IsActive;
	SetActorHiddenInGame(!IsActive);
}

void AABBoss::Deactivate() //��Ƽ��
{
	this->SetActive(false);
	SetActorEnableCollision(false);
	Cast<UABGameInstance>(GetGameInstance())->SetBossPTR(nullptr);
}

bool AABBoss::IsActive()
{
	return Active;
}

void AABBoss::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("ARROW")))
	{
		AArrow* Arrow = dynamic_cast<AArrow*>(OtherActor);
		BossHP -= Arrow->Damage;
		UE_LOG(LogTemp, Error, TEXT("BossHP : %f"), BossHP);
		Arrow->SetActive(false);
		Arrow->SetActorLocation(FVector(0, 0, 0));
		
	}
}
