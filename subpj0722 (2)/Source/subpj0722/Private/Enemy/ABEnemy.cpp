// Fill out your copyright notice in the Description page of Project Settings.


#include "ABEnemy.h"
#include "Arrow.h"
#include "ABGameInstance.h"
#include "ABCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AABEnemy::AABEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(false); //오브젝트풀이 스폰하면서 액티브 false를 하지만 여기서도 해준다.
	// 스켈레탈 매시 위치,회전 조정
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0)); 
	GetMesh()->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	GetCapsuleComponent()->bHasPerInstanceHitProxies = true; 

	//스켈레탈 메시 셋
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/IronMaskSK")); 
	if (SK_BODY.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("Set Enemy Body"));
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	
	//애니메이션 셋
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>ENEMY_ANIM(TEXT("/Game/Mynew/EnemyAnim/ABEnemyAnimBlueprint"));
	if (ENEMY_ANIM.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("Set Enemy Anim"));
		GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);
	}

	//캡슐컴포넌트 태그 추가
	GetCapsuleComponent()->ComponentTags.Add(FName("ENEMY"));
}

// Called when the game starts or when spawned
void AABEnemy::BeginPlay()
{
	Super::BeginPlay();
	HP = 200.0f;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AABEnemy::OnActorBeginOverlap1);
	GetCharacterMovement()->MaxWalkSpeed = 1200; //돌진속도 1200 걷기속도의 2배
}

void AABEnemy::Deactivate()
{
	this->SetActive(false);
	SetActorEnableCollision(false);
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));//캐릭터가 죽었으니 함수를 호출
	if (temp == nullptr)
	{
		return;
	}
	Cast<UABGameInstance>(GetGameInstance())->CharacterCallToken(); // 게임인스턴스의 캐릭찾기 델리게이트 호출함수
	temp->GetWorldTimerManager().SetTimer(temp->FireTimerHandler1, temp, &AABCharacter::FindEnemy, 0.1f, true); //캐릭터의 캐릭찾기 타이머 셋
}

// Called every frame
void AABEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookCharacter();
	if (IsActive())
	{
		if (HP <= 0)
		{
			ticktime++;
			if (ticktime >= 50) //애니메이션 보여주고 디엑티브하기위해서
			{
				if (DoOnce)
				{
					DoOnce = false;
					GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					Deactivate();
					Cast<UABGameInstance>(GetGameInstance())->SetDeadEnemyptr(this);
					Cast<UABGameInstance>(GetGameInstance())->SpawnExpBallfunc(); //적이 죽었으니 죽은자리에 경험치볼을 스폰하기위한 함수 실행
				}
			}
		}
	}
}

//활성화 또는 비활성화 셋
void AABEnemy::SetActive(bool InActive)
{
	Active = InActive;
	SetActorHiddenInGame(!InActive);
}

//활성화 여부 리턴
bool AABEnemy::IsActive() 
{
	return Active;
}


//화살과 비긴오버랩 함수~
void AABEnemy::OnActorBeginOverlap1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("ARROW")))
	{
		AArrow* Arrow = dynamic_cast<AArrow*>(OtherActor);
		HP -= Arrow->Damage;
		UE_LOG(LogTemp, Error, TEXT("HP : %f"), HP);
		Ishit = true; //피격시 트루
		if (Cast<UABGameInstance>(GetGameInstance())->bGIPenetrate == false)
		{
			Arrow->SetActive(false);
			Arrow->SetActorLocation(FVector(0, 0, 0));
		}
	}
}

// 캐릭터 처다보기
void AABEnemy::LookCharacter()
{
	Enemyloc = GetMesh()->GetComponentLocation();
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (temp != nullptr)
	{
		Characterloc = temp->GetMesh()->GetComponentLocation();
		CharacterRot = UKismetMathLibrary::FindLookAtRotation(Enemyloc, Characterloc) + FRotator(0, -90, 0);
		GetMesh()->SetWorldRotation(CharacterRot);
	}
}