// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "WeaponBow.h"
#include "ABGameInstance.h"
#include "TableManager.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AABCharacter::AABCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//ī�޶� �������� ��
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	SetControlMode(); //��Ʈ�ѷ� ī�޶������� ����


	 //���̷�Ż�޽ü�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BOWMAN(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/CountSK"));
	if (SK_BOWMAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BOWMAN.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f)); //��Ķ��Ż�޽� ��ġ ȸ�� ����


	//�ִ� �������Ʈ ��
	static ConstructorHelpers::FClassFinder<UAnimInstance> BOWMAN_ANIM(TEXT("/Game/Mynew/CharacterAnim/NewAnimBlueprint")); 
	if (BOWMAN_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BOWMAN_ANIM.Class);
	}



	//HP�� 
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("CHAR_HPBAR"));
	HPBar->SetupAttachment(RootComponent);
	HPBar->SetWorldLocation(FVector(0, 0, 150.0f));
	ConstructorHelpers::FClassFinder<UUserWidget>UW_HPBAR(TEXT("/Game/Mynew/CharacterHUD/BP_CharacterHPbar"));
	if (UW_HPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UW_HPBAR.Class);
	}
	HPBar->SetRenderCustomDepth(true);
	
	// ȭ�� Ǯ
	ArrowPooler = CreateDefaultSubobject<UArrowPool>(TEXT("ArrowPool"));

	//�缱ȭ�� �߻����
	DiagonalPos1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos1")); //�缱ȭ���� ���ư� ������ staticmeshcomponent�� �������� ĳ������ �����ʴ밢��
	DiagonalPos2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos2")); //�缱ȭ���� ���ư� ������ staticmeshcomponent�� �������� ĳ������ ���ʴ밢��
	DiagonalPos3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos3")); //�缱ȭ���� ���ư� ������ staticmeshcomponent�� �������� ĳ�� ������
	DiagonalPos4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos4")); //�缱ȭ���� ���ư� ������ staticmeshcomponent�� ��������	ĳ�� ����

	
	DiagonalPos1->SetWorldLocation(FVector(500, 500, 0));
	DiagonalPos1->SetupAttachment(RootComponent);
	DiagonalPos2->SetWorldLocation(FVector(500, -500, 0));
	DiagonalPos2->SetupAttachment(RootComponent);
	DiagonalPos3->SetWorldLocation(FVector(0, 500, 0));
	DiagonalPos3->SetupAttachment(RootComponent);
	DiagonalPos4->SetWorldLocation(FVector(0, -500, 0));
	DiagonalPos4->SetupAttachment(RootComponent);

  }

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	//���� ����
	auto CurWeapon = GetWorld()->SpawnActor<AWeaponBow>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon != nullptr)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
	}

	//ĸ��������Ʈ ����
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AABCharacter::OnActorBeginOverlap2);
	GetCapsuleComponent()->ComponentTags.Add(FName("PLAYER"));


	//ĳ���� ����ġ, ��ų, ���� �����ν��Ͻ����� �ҷ��� �ʱ�ȭ
	CurrentHP = Cast<UABGameInstance>(GetGameInstance())->CharCurrentHP;
	CurrentEXP = Cast<UABGameInstance>(GetGameInstance())->InsCurrentExp;
	CurrentLevel = Cast<UABGameInstance>(GetGameInstance())->InsCurrentLevel;
	MaxEXP = Cast<UABGameInstance>(GetGameInstance())->InsCurrentMaxExp;
	MaxLevel = Cast<UABGameInstance>(GetGameInstance())->MaxLevel;

	bDiagonal = Cast<UABGameInstance>(GetGameInstance())->bGIDiagonal;
	bMulti = Cast<UABGameInstance>(GetGameInstance())->bGIMulti;
	bRear = Cast<UABGameInstance>(GetGameInstance())->bGIRear;
	bRightAndLeft = Cast<UABGameInstance>(GetGameInstance())->bGIRightAndLeft;
	bPenetrateShot = Cast<UABGameInstance>(GetGameInstance())->bGIPenetrate;

	Boss = Cast<UABGameInstance>(GetGameInstance())->BossPTR;

	Fire(); //�����Ҷ��� �ѹ� �߻��Լ�����
	// ���۽� �߻� �Լ��� ���� �ʿ� �����ְ� �����ڸ����� �������� ������ �߻����� ���� 
}

void AABCharacter::SetControlMode()
{
	SpringArm->TargetArmLength = 2800.0f;
	SpringArm->SetRelativeRotation(FRotator(-80.0f,0.0f,0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 3000;
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WidgetRot = UKismetMathLibrary::FindLookAtRotation(Charloc, FVector((-15960.000,0,13700.000)));//������ ī�޶��� ��ġ���� �� �հŸ��� �ٶ󺸰� ����� ������ ȸ���� ������Ų��. ����ٲ����ʴ´�
	HPBar->SetWorldRotation(WidgetRot+FRotator(0.0f,137.5f,0.0f)); // ������ �������������� �� ���ư��� �׷��� ȸ�������� �� ���ؼ� ���ȴ�.

	BPShotCheck = false;
	//UE_LOG(LogTemp,Error,TEXT("x = %f, y = %f, z = %f"), GetActorForwardVector().X, GetActorForwardVector().Y, GetActorForwardVector().Z);
	if (DirectionToMove.SizeSquared() > 0.0f) //ĳ���� �����̱�
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator()); //ĳ������ ȸ��
		AddMovementInput(DirectionToMove); //ĳ���� ������ �յ��¿�
	}

	if(CurrentHP<=0)//�׾��ٸ� �߻� ȸ�� ��� �� ����
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();

		GetWorldTimerManager().ClearTimer(FireTimerHandler1);
		GetWorldTimerManager().ClearTimer(FireTimerHandler2);
		GetWorldTimerManager().ClearTimer(FireTimerHandler3);
		GetWorldTimerManager().ClearTimer(FireTimerHandler4);
		GetWorldTimerManager().ClearTimer(FireTimerHandler5);

	}
}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AABCharacter::NoFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AABCharacter::Fire);

	PlayerInputComponent->BindAction(TEXT("1"), IE_Pressed, this, &AABCharacter::SetDiagonalShot); //�ӽü��� ������ ������ ������ ������ 3���� ��ų�� �ϳ��� �����ؼ� ����� �� �ֵ��� ������ �ؾ� ������
	PlayerInputComponent->BindAction(TEXT("2"), IE_Pressed, this, &AABCharacter::SetMultiShot);		// ���ǻ� 12345 ���� ��ų�� �Ҵ��ؼ� �����ְ� �ӽü��� �Ͽ���
	PlayerInputComponent->BindAction(TEXT("3"), IE_Pressed, this, &AABCharacter::SetRearShot);
	PlayerInputComponent->BindAction(TEXT("4"), IE_Pressed, this, &AABCharacter::SetRightAndLeftShot);
	PlayerInputComponent->BindAction(TEXT("5"), IE_Pressed, this, &AABCharacter::SetPentrateShot);

}



void AABCharacter::UpDown(float NewAxisValue) //���� ������ �� ��
{
	DirectionToMove.X = NewAxisValue;
}

void AABCharacter::LeftRight(float NewAxisValue) //�¿� ������ �� ��
{
	DirectionToMove.Y = NewAxisValue;
}

void AABCharacter::NoFire() //���� �����̴���
{

	BPShotCheck = false;  //�����δٸ� ���ϸ��̼��� �������� false
	GetWorldTimerManager().ClearTimer(FireTimerHandler1);//Ÿ�̸� �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(FireTimerHandler2);//Ÿ�̸� �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(FireTimerHandler3);//Ÿ�̸� �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(FireTimerHandler4);//Ÿ�̸� �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(FireTimerHandler5);//Ÿ�̸� �ʱ�ȭ
}
void AABCharacter::Fire() //������� ��������
{
	Cast<UABGameInstance>(GetGameInstance())->CharacterCallToken(); //�����ν��Ͻ��� ��ĳ���͸� ã����� �Լ�ȣ��
	GetWorldTimerManager().SetTimer(FireTimerHandler1, this, &AABCharacter::FindEnemy, 0.1f, true);
	GetWorldTimerManager().SetTimer(FireTimerHandler2, this,&AABCharacter::Shot, ShotDulation, true);
	GetWorldTimerManager().SetTimer(FireTimerHandler4, this, &AABCharacter::FindBoss, 0.1f, true);
	GetWorldTimerManager().SetTimer(FireTimerHandler5, this, &AABCharacter::BossShot, ShotDulation, true);

	if (bMulti)
	{
		MultiShot();
	}
}

void AABCharacter::FindEnemy()
{
	Charloc = GetMesh()->GetComponentLocation(); //ĳ������ ��ġ(�޽��� ��������̼�)
	if (Cast<UABGameInstance>(GetGameInstance())->InsEnemy != nullptr)
	{
		Enemyloc = Cast<UABGameInstance>(GetGameInstance())->InsEnemy->GetMesh()->GetComponentLocation(); //���� ��ġ (�����ν��Ͻ��� �޾Ƴ� ���� �����ͷ� ���� ��ġ)
		IsEnemyExist = true;
		EnemyRot = UKismetMathLibrary::FindLookAtRotation(Charloc, Enemyloc);
		EnemyRot.Normalize();
		GetController()->SetControlRotation(EnemyRot);
	}
	else
	{
		//GetWorldTimerManager().ClearTimer(FireTimerHandler1);
		GetWorldTimerManager().ClearTimer(FireTimerHandler2);
		GetWorldTimerManager().ClearTimer(FireTimerHandler3);
		IsEnemyExist = false;
		UE_LOG(LogTemp, Error, TEXT("NoEnmey"));
		NoEnemyInField.Broadcast(); //����ġ������ �ݸ����� �ٲ��ְ� ĳ���ͷ� �̵���Ű�� ���� ��������Ʈ
	}

}
void AABCharacter::FindBoss() //����ã��
{
	Charloc = GetMesh()->GetComponentLocation(); //ĳ������ ��ġ(�޽��� ��������̼�)

	if (Boss != nullptr)
	{
		if (Boss->IsActive())
		{
			Enemyloc = Boss->GetMesh()->GetComponentLocation();
			IsBossExist = true;
			EnemyRot = UKismetMathLibrary::FindLookAtRotation(Charloc, Enemyloc);
			EnemyRot.Normalize();
			GetController()->SetControlRotation(EnemyRot);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(FireTimerHandler4);
			IsBossExist = false;
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandler4);
		IsBossExist = false;
	}
}


void AABCharacter::Shot()
{
	if (IsEnemyExist)
	{
		AArrow* Arrow = ArrowPooler->GetArrowPooledObject();
		if (Arrow != nullptr)
		{
			Arrow->SetActive(true);
			Arrow->SetActorLocation(GetMesh()->GetComponentLocation()+FVector(0.0f, 0.0f, 120.0f)+ GetActorForwardVector()*100);
			Arrow->SetActorRotation(EnemyRot);
			Arrow->ProjectileMovenent->Velocity = GetActorForwardVector()*Arrow->ProjectileMovenent->InitialSpeed;
		}

		if (bDiagonal)
		{
			DiagonalShot();
		}
		if (bRear)
		{
			RearShot();
		}
		if (bRightAndLeft)
		{
			RightAndLeftShot();
		}
		
		//�߻�ִϸ��̼� �����ϱ����� Ʈ��
		BPShotCheck = true;
	}
}

void AABCharacter::BossShot()
{
	if (IsBossExist)
	{
		AArrow* Projectile = ArrowPooler->GetArrowPooledObject();
		if (Projectile != nullptr)
		{
			Projectile->SetActive(true);
			Projectile->SetActorLocation(GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100);
			Projectile->SetActorRotation(EnemyRot);
			Projectile->ProjectileMovenent->Velocity = GetActorForwardVector()*Projectile->ProjectileMovenent->InitialSpeed;
		}
		Projectile->Damage *= 2;
		if (bPenetrateShot)
		{
			PenetrateShot(Projectile);
		}
		if (bDiagonal)
		{
			DiagonalShot();
		}
		if (bRear)
		{
			RearShot();
		}
		if (bRightAndLeft)
		{
			RightAndLeftShot();
		}
		//�߻�ִϸ��̼� �����ϱ����� Ʈ��
		BPShotCheck = true;
	}
}

void AABCharacter::DiagonalShot() //�缱���
{
	AArrow* Projectile1 = ArrowPooler->GetArrowPooledObject();
	if (Projectile1 != nullptr)
	{
		FVector Firepos1 = (DiagonalPos1->GetComponentLocation() - GetMesh()->GetComponentLocation());// ȭ���� ���ư� ���⿡�� ĳ���� �Ž��� ��ġ���� ���� ���ư� ������͸� ���Ѵ�.
		Firepos1.Z = 0;
		Projectile1->SetActive(true);
		Projectile1->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile1->ProjectileMovenent->Velocity = Firepos1 * Projectile1->ProjectileMovenent->InitialSpeed/500;//������Ϳ� �ӵ����� ���ؼ� �ӵ��� ���Ѵ�.(�ӵ�:����� �ӷ��� ��������) 500�� ���������� �ӵ��� �ʹ����� ����
	}
	AArrow* Projectile2 = ArrowPooler->GetArrowPooledObject();
	if (Projectile2 != nullptr)
	{
		FVector Firepos2 = (DiagonalPos2->GetComponentLocation() - GetMesh()->GetComponentLocation());// ȭ���� ���ư� ���⿡�� ĳ���� �Ž��� ��ġ���� ���� ���ư� ������͸� ���Ѵ�.
		Firepos2.Z = 0;
		Projectile2->SetActive(true);
		Projectile2->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile2->ProjectileMovenent->Velocity = Firepos2*Projectile2->ProjectileMovenent->InitialSpeed/500;//������Ϳ� �ӵ����� ���ؼ� �ӵ��� ���Ѵ�.(�ӵ�:����� �ӷ��� ��������) 500�� ���������� �ӵ��� �ʹ����� ����
	}
	if (bPenetrateShot)
	{
		PenetrateShot(Projectile1);
		PenetrateShot(Projectile2);
	}
	
}

void AABCharacter::SetDiagonalShot()
{
	if (bDiagonal == false)
	{
		bDiagonal = true;
		Cast<UABGameInstance>(GetGameInstance())->bGIDiagonal = true;
	}
	else
	{
		bDiagonal = false;
		Cast<UABGameInstance>(GetGameInstance())->bGIDiagonal = false;
	}
}

void AABCharacter::MultiShot() //��Ƽ��
{
	GetWorldTimerManager().SetTimer(FireTimerHandler3, this, &AABCharacter::Shot, ShotDulation / 2.0f, true);
	GetWorldTimerManager().SetTimer(FireTimerHandler5, this, &AABCharacter::BossShot, ShotDulation / 2.0f, true);

}

void AABCharacter::SetMultiShot()
{
	if (bMulti == false)
	{
		bMulti = true;
		Cast<UABGameInstance>(GetGameInstance())->bGIMulti = true;
	}
	else
	{
		bMulti = false;
		Cast<UABGameInstance>(GetGameInstance())->bGIMulti = false;
	}
}

void AABCharacter::RearShot() //�Ĺ���
{
	UWorld* World = GetWorld();

	if (World)
	{

		AArrow* Projectile3 = ArrowPooler->GetArrowPooledObject();
		if (Projectile3 != nullptr)
		{
			Projectile3->SetActive(true);
			Projectile3->SetActorLocation(GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100);
			Projectile3->SetActorRotation(EnemyRot);
			Projectile3->ProjectileMovenent->Velocity = -GetActorForwardVector()*Projectile3->ProjectileMovenent->InitialSpeed;
		}
		if (bPenetrateShot)
		{
			PenetrateShot(Projectile3);
		}
	}
}

void AABCharacter::SetRearShot()
{
	if (bRear == false)
	{
		bRear = true;
		Cast<UABGameInstance>(GetGameInstance())->bGIRear = true;
	}
	else
	{
		bRear = false;
		Cast<UABGameInstance>(GetGameInstance())->bGIRear = false;
	}
}

void AABCharacter::RightAndLeftShot() //�¿���
{
	
	AArrow* Projectile4 = ArrowPooler->GetArrowPooledObject();
	if (Projectile4 != nullptr)
	{
		FVector Firepos4 = (DiagonalPos3->GetComponentLocation() - GetMesh()->GetComponentLocation());// ȭ���� ���ư� ���⿡�� ĳ���� �Ž��� ��ġ���� ���� ���ư� ������͸� ���Ѵ�.
		Firepos4.Z = 0;
		Projectile4->SetActive(true);
		Projectile4->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile4->ProjectileMovenent->Velocity = Firepos4 * Projectile4->ProjectileMovenent->InitialSpeed / 500;//������Ϳ� �ӵ����� ���ؼ� �ӵ��� ���Ѵ�.(�ӵ�:����� �ӷ��� ��������) 500�� ���������� �ӵ��� �ʹ����� ����
	}
	AArrow* Projectile5 = ArrowPooler->GetArrowPooledObject();
	if (Projectile5 != nullptr)
	{
		FVector Firepos5 = (DiagonalPos4->GetComponentLocation() - GetMesh()->GetComponentLocation());// ȭ���� ���ư� ���⿡�� ĳ���� �Ž��� ��ġ���� ���� ���ư� ������͸� ���Ѵ�.
		Firepos5.Z = 0;
		Projectile5->SetActive(true);
		Projectile5->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile5->ProjectileMovenent->Velocity = Firepos5 * Projectile5->ProjectileMovenent->InitialSpeed / 500;//������Ϳ� �ӵ����� ���ؼ� �ӵ��� ���Ѵ�.(�ӵ�:����� �ӷ��� ��������) 500�� ���������� �ӵ��� �ʹ����� ����
	}
	if (bPenetrateShot)
	{
			PenetrateShot(Projectile4);
			PenetrateShot(Projectile5);
	}
	
}

void AABCharacter::SetRightAndLeftShot()
{
	if (bRightAndLeft == false)
	{
		bRightAndLeft = true;
		Cast<UABGameInstance>(GetGameInstance())->bGIRightAndLeft = true;
	}
	else
	{
		bRightAndLeft = false;
		Cast<UABGameInstance>(GetGameInstance())->bGIRightAndLeft = false;
	}
}

void AABCharacter::PenetrateShot(AArrow* Arrowptr)
{
	Arrowptr->Arrow->SetCollisionProfileName("PenetrateProjectile");
}

void AABCharacter::SetPentrateShot()
{
	if (bPenetrateShot==false)
	{
		bPenetrateShot = true;
		Cast<UABGameInstance>(GetGameInstance())->bGIPenetrate = true;
	}
	else
	{
		bPenetrateShot = false;
		Cast<UABGameInstance>(GetGameInstance())->bGIPenetrate = false;
	}
}


void AABCharacter::OnActorBeginOverlap2(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("MagicBall")))
	{
		CurrentHP -= MagicBallDamage;
		Cast<UABGameInstance>(GetGameInstance())->SetCurrentHP(CurrentHP);

		
	}
	else if (OtherComp->ComponentHasTag(FName("Axe")))
	{
		
		CurrentHP -= AxeDamage;
		Cast<UABGameInstance>(GetGameInstance())->SetCurrentHP(CurrentHP);
	}
	else if (OtherComp->ComponentHasTag(FName("BossSword")))
	{
		CurrentHP -= AxeDamage*30;
		Cast<UABGameInstance>(GetGameInstance())->SetCurrentHP(CurrentHP);

	}
	if (OtherComp->ComponentHasTag(FName("ExpBall"))) //����ġ���� �����ϸ�
	{
		CurrentEXP += ExpAmount; //����ġ���� ����ġ����ŭ ���� ĳ������ ����ġ�� ��½�Ų��.
		Cast<UABGameInstance>(GetGameInstance())->SetCurrentExp(CurrentEXP); //�����ν��Ͻ������� ���� ĳ���� ����ġ�� ��½�Ų��.
		if (CurrentEXP == MaxEXP) //���� �������ġ�� ������ �ִ����ġ�� �����ϸ�
		{
			CurrentEXP = 0; //�������ġ�� 0���θ����
			CurrentLevel++; //������ 1 �÷��ش�.
			Cast<UABGameInstance>(GetGameInstance())->SetCurrentExp(CurrentEXP); //�����ν��Ͻ��� �������ġ���� ĳ������ ����ġ���� ����ȭ�Ѵ�.
			Cast<UABGameInstance>(GetGameInstance())->SetCurrentLevel(CurrentLevel); //�����ν��Ͻ��� ���緹���� ĳ������ ������ ����ȭ�Ѵ�.
			Cast<UABGameInstance>(GetGameInstance())->SetCurrentMaxExp(CurrentLevel); //�����ν��Ͻ����� �ö� ������ �°� �ִ����ġ�� �ٽ� �����Ѵ�.
			MaxEXP = Cast<UABGameInstance>(GetGameInstance())->InsCurrentMaxExp; //ĳ������ �ִ����ġ���� ������ �°� �ٽ� �����Ѵ�.

		}
	}
	
}


