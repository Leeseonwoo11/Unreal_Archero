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

	SetActorEnableCollision(false); //오브젝트풀이 스폰할때 비활성화 상태이긴하지만 콜리젼을 false로 초기화 해줌

	// 스켈레탈 메시 회전,위치,태그 셋
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	GetMesh()->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	GetMesh()->SetCollisionProfileName(FName("EnemyBody"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/ExecutionerSK"));
	if (SK_BODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	// 애니메이션 셋
	ConstructorHelpers::FClassFinder<UAnimInstance>ANIM_ABBOSS(TEXT("/Game/Mynew/EnemyAnim/ABBossAnimBlueprint"));
	if (ANIM_ABBOSS.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_ABBOSS.Class);
	}
	//AI컨트롤러 셋 (기본 AIController)
	AIControllerClass = AABBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// 컨트롤러 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 480.0f, 0);
}

// Called when the game starts or when spawned
void AABBoss::BeginPlay()
{
	Super::BeginPlay();
	
	//캡슐컴포넌트 오버랩 함수 바인드
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AABBoss::OnActorBeginOverlap);
}


// Called every frame
void AABBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->IsActive()) //활성화 상태라면 캐릭터를 처다본다. 
	{
		LookCharacter(); 
	}
	if (BossHP <= 0) //피가 0보다 작아지면
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
		GetCharacterMovement()->MaxWalkSpeed = 400.0f; // 평상시 보스 걷기 속도
	}
}

void AABBoss::LookCharacter() // 플레이어 처다보기
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

void AABBoss::RushAttack() //돌격공격
{
	if (this->IsActive())
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.0f; // 속도 3배 올려줌
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

void AABBoss::RushAttackEnd() //돌격 공격 종료
{
	if (this->IsActive())
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f; //속도 다시 낮춰놓음
		IsRush = false;
		OnRushAttackEnd.Broadcast(); // BossAI의 BTTaskNode_RushAttack.cpp(26)에 바인드 되어있음 (비헤이비어 트리)
	}
}

void AABBoss::MeleeAttack() //근접공격
{
	if (this->IsActive())
	{
		IsMelee = true;
		FTimerHandle MeleeAttackTimer;
		GetWorldTimerManager().SetTimer(MeleeAttackTimer, this, &AABBoss::MeleeAttackEnd, 5.0, false, 1.0f);
	}
}

void AABBoss::MeleeAttackEnd() //근접공격 종료
{
	if (this->IsActive())
	{
		IsMelee = false;
		OnMeleeAttackEnd.Broadcast(); //BossAI의 BTTaskNodeMelee.cpp(25)에 바인드 되어있음(비헤이비어 트리)
	}
}

void AABBoss::FlameAttack() //원거리 검기공격
{
	if (this->IsActive())
	{
		IsFlame = true;
		AABCharacter* ABCharacter = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); //플레이어의 포인터
		if (ABCharacter == nullptr)
		{
			return;
		}
		UWorld* World = GetWorld();

		if (World) //스폰한다.(캐릭터에 발사한다.)
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

void AABBoss::SetActive(bool IsActive)//활성 또는 비활성
{
	Active = IsActive;
	SetActorHiddenInGame(!IsActive);
}

void AABBoss::Deactivate() //디엑티브
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
