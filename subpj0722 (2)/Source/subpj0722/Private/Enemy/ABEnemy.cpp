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
	SetActorEnableCollision(false); //������ƮǮ�� �����ϸ鼭 ��Ƽ�� false�� ������ ���⼭�� ���ش�.
	// ���̷�Ż �Ž� ��ġ,ȸ�� ����
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0)); 
	GetMesh()->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	GetCapsuleComponent()->bHasPerInstanceHitProxies = true; 

	//���̷�Ż �޽� ��
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/IronMaskSK")); 
	if (SK_BODY.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("Set Enemy Body"));
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	
	//�ִϸ��̼� ��
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>ENEMY_ANIM(TEXT("/Game/Mynew/EnemyAnim/ABEnemyAnimBlueprint"));
	if (ENEMY_ANIM.Succeeded())
	{
		UE_LOG(LogTemp, Error, TEXT("Set Enemy Anim"));
		GetMesh()->SetAnimInstanceClass(ENEMY_ANIM.Class);
	}

	//ĸ��������Ʈ �±� �߰�
	GetCapsuleComponent()->ComponentTags.Add(FName("ENEMY"));
}

// Called when the game starts or when spawned
void AABEnemy::BeginPlay()
{
	Super::BeginPlay();
	HP = 200.0f;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AABEnemy::OnActorBeginOverlap1);
	GetCharacterMovement()->MaxWalkSpeed = 1200; //�����ӵ� 1200 �ȱ�ӵ��� 2��
}

void AABEnemy::Deactivate()
{
	this->SetActive(false);
	SetActorEnableCollision(false);
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));//ĳ���Ͱ� �׾����� �Լ��� ȣ��
	if (temp == nullptr)
	{
		return;
	}
	Cast<UABGameInstance>(GetGameInstance())->CharacterCallToken(); // �����ν��Ͻ��� ĳ��ã�� ��������Ʈ ȣ���Լ�
	temp->GetWorldTimerManager().SetTimer(temp->FireTimerHandler1, temp, &AABCharacter::FindEnemy, 0.1f, true); //ĳ������ ĳ��ã�� Ÿ�̸� ��
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
			if (ticktime >= 50) //�ִϸ��̼� �����ְ� ��Ƽ���ϱ����ؼ�
			{
				if (DoOnce)
				{
					DoOnce = false;
					GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					Deactivate();
					Cast<UABGameInstance>(GetGameInstance())->SetDeadEnemyptr(this);
					Cast<UABGameInstance>(GetGameInstance())->SpawnExpBallfunc(); //���� �׾����� �����ڸ��� ����ġ���� �����ϱ����� �Լ� ����
				}
			}
		}
	}
}

//Ȱ��ȭ �Ǵ� ��Ȱ��ȭ ��
void AABEnemy::SetActive(bool InActive)
{
	Active = InActive;
	SetActorHiddenInGame(!InActive);
}

//Ȱ��ȭ ���� ����
bool AABEnemy::IsActive() 
{
	return Active;
}


//ȭ��� �������� �Լ�~
void AABEnemy::OnActorBeginOverlap1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("ARROW")))
	{
		AArrow* Arrow = dynamic_cast<AArrow*>(OtherActor);
		HP -= Arrow->Damage;
		UE_LOG(LogTemp, Error, TEXT("HP : %f"), HP);
		Ishit = true; //�ǰݽ� Ʈ��
		if (Cast<UABGameInstance>(GetGameInstance())->bGIPenetrate == false)
		{
			Arrow->SetActive(false);
			Arrow->SetActorLocation(FVector(0, 0, 0));
		}
	}
}

// ĳ���� ó�ٺ���
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