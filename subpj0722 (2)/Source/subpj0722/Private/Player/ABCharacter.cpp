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


	//카메라 스프링암 셋
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	SetControlMode(); //컨트롤러 카메라스프링암 설정


	 //스켈레탈메시셋
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BOWMAN(TEXT("/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/CountSK"));
	if (SK_BOWMAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BOWMAN.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f)); //스캘레탈메시 위치 회전 조정


	//애님 블루프린트 셋
	static ConstructorHelpers::FClassFinder<UAnimInstance> BOWMAN_ANIM(TEXT("/Game/Mynew/CharacterAnim/NewAnimBlueprint")); 
	if (BOWMAN_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BOWMAN_ANIM.Class);
	}



	//HP바 
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("CHAR_HPBAR"));
	HPBar->SetupAttachment(RootComponent);
	HPBar->SetWorldLocation(FVector(0, 0, 150.0f));
	ConstructorHelpers::FClassFinder<UUserWidget>UW_HPBAR(TEXT("/Game/Mynew/CharacterHUD/BP_CharacterHPbar"));
	if (UW_HPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UW_HPBAR.Class);
	}
	HPBar->SetRenderCustomDepth(true);
	
	// 화살 풀
	ArrowPooler = CreateDefaultSubobject<UArrowPool>(TEXT("ArrowPool"));

	//사선화살 발사방향
	DiagonalPos1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos1")); //사선화살이 날아갈 방향을 staticmeshcomponent로 만들어놓음 캐릭전방 오른쪽대각선
	DiagonalPos2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos2")); //사선화살이 날아갈 방향을 staticmeshcomponent로 만들어놓음 캐릭전방 왼쪽대각선
	DiagonalPos3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos3")); //사선화살이 날아갈 방향을 staticmeshcomponent로 만들어놓음 캐릭 오른쪽
	DiagonalPos4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DiagonalPos4")); //사선화살이 날아갈 방향을 staticmeshcomponent로 만들어놓음	캐릭 왼쪽

	
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

	//무기 장착
	auto CurWeapon = GetWorld()->SpawnActor<AWeaponBow>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (CurWeapon != nullptr)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
	}

	//캡슐컴포넌트 설정
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AABCharacter::OnActorBeginOverlap2);
	GetCapsuleComponent()->ComponentTags.Add(FName("PLAYER"));


	//캐릭터 경험치, 스킬, 레벨 게임인스턴스에서 불러와 초기화
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

	Fire(); //시작할때만 한번 발사함수실행
	// 시작시 발사 함수가 없고 맵에 적이있고 시작자리에서 움직이지 않으면 발사하지 않음 
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
	WidgetRot = UKismetMathLibrary::FindLookAtRotation(Charloc, FVector((-15960.000,0,13700.000)));//위젯이 카메라의 위치보다 더 먼거리를 바라보게 만들어 위젯의 회전을 고정시킨다. 절대바뀌지않는다
	HPBar->SetWorldRotation(WidgetRot+FRotator(0.0f,137.5f,0.0f)); // 돌려도 무슨이유인지모름 덜 돌아간다 그래서 회전각도를 더 더해서 돌렸다.

	BPShotCheck = false;
	//UE_LOG(LogTemp,Error,TEXT("x = %f, y = %f, z = %f"), GetActorForwardVector().X, GetActorForwardVector().Y, GetActorForwardVector().Z);
	if (DirectionToMove.SizeSquared() > 0.0f) //캐릭터 움직이기
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator()); //캐릭터의 회전
		AddMovementInput(DirectionToMove); //캐릭터 움직임 앞뒤좌우
	}

	if(CurrentHP<=0)//죽었다면 발사 회전 등등 다 꺼줌
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

	PlayerInputComponent->BindAction(TEXT("1"), IE_Pressed, this, &AABCharacter::SetDiagonalShot); //임시설정 원래는 레벨업 했을때 랜덤한 3개의 스킬중 하나를 선택해서 사용할 수 있도록 구현을 해야 했으나
	PlayerInputComponent->BindAction(TEXT("2"), IE_Pressed, this, &AABCharacter::SetMultiShot);		// 편의상 12345 번에 스킬을 할당해서 쓸수있게 임시설정 하였음
	PlayerInputComponent->BindAction(TEXT("3"), IE_Pressed, this, &AABCharacter::SetRearShot);
	PlayerInputComponent->BindAction(TEXT("4"), IE_Pressed, this, &AABCharacter::SetRightAndLeftShot);
	PlayerInputComponent->BindAction(TEXT("5"), IE_Pressed, this, &AABCharacter::SetPentrateShot);

}



void AABCharacter::UpDown(float NewAxisValue) //상하 움직임 값 셋
{
	DirectionToMove.X = NewAxisValue;
}

void AABCharacter::LeftRight(float NewAxisValue) //좌우 움직임 값 셋
{
	DirectionToMove.Y = NewAxisValue;
}

void AABCharacter::NoFire() //못쏨 움직이는중
{

	BPShotCheck = false;  //움직인다면 에니메이션을 끄기위해 false
	GetWorldTimerManager().ClearTimer(FireTimerHandler1);//타이머 초기화
	GetWorldTimerManager().ClearTimer(FireTimerHandler2);//타이머 초기화
	GetWorldTimerManager().ClearTimer(FireTimerHandler3);//타이머 초기화
	GetWorldTimerManager().ClearTimer(FireTimerHandler4);//타이머 초기화
	GetWorldTimerManager().ClearTimer(FireTimerHandler5);//타이머 초기화
}
void AABCharacter::Fire() //쏠수있음 멈춰있음
{
	Cast<UABGameInstance>(GetGameInstance())->CharacterCallToken(); //게임인스턴스에 적캐릭터를 찾으라고 함수호출
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
	Charloc = GetMesh()->GetComponentLocation(); //캐릭터의 위치(메시의 월드로케이션)
	if (Cast<UABGameInstance>(GetGameInstance())->InsEnemy != nullptr)
	{
		Enemyloc = Cast<UABGameInstance>(GetGameInstance())->InsEnemy->GetMesh()->GetComponentLocation(); //적의 위치 (게임인스턴스에 받아논 적의 포인터로 받은 위치)
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
		NoEnemyInField.Broadcast(); //경험치볼들의 콜리젼을 바꿔주고 캐릭터로 이동시키기 위한 델리게이트
	}

}
void AABCharacter::FindBoss() //보스찾기
{
	Charloc = GetMesh()->GetComponentLocation(); //캐릭터의 위치(메시의 월드로케이션)

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
		
		//발사애니메이션 실행하기위한 트루
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
		//발사애니메이션 실행하기위한 트루
		BPShotCheck = true;
	}
}

void AABCharacter::DiagonalShot() //사선사격
{
	AArrow* Projectile1 = ArrowPooler->GetArrowPooledObject();
	if (Projectile1 != nullptr)
	{
		FVector Firepos1 = (DiagonalPos1->GetComponentLocation() - GetMesh()->GetComponentLocation());// 화살이 날아갈 방향에서 캐릭터 매시의 위치값을 빼서 날아갈 방향백터를 구한다.
		Firepos1.Z = 0;
		Projectile1->SetActive(true);
		Projectile1->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile1->ProjectileMovenent->Velocity = Firepos1 * Projectile1->ProjectileMovenent->InitialSpeed/500;//방향백터와 속도값을 곱해서 속도를 구한다.(속도:방향과 속력이 같이있음) 500을 나눈이유는 속도가 너무빨라서 나눔
	}
	AArrow* Projectile2 = ArrowPooler->GetArrowPooledObject();
	if (Projectile2 != nullptr)
	{
		FVector Firepos2 = (DiagonalPos2->GetComponentLocation() - GetMesh()->GetComponentLocation());// 화살이 날아갈 방향에서 캐릭터 매시의 위치값을 빼서 날아갈 방향백터를 구한다.
		Firepos2.Z = 0;
		Projectile2->SetActive(true);
		Projectile2->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile2->ProjectileMovenent->Velocity = Firepos2*Projectile2->ProjectileMovenent->InitialSpeed/500;//방향백터와 속도값을 곱해서 속도를 구한다.(속도:방향과 속력이 같이있음) 500을 나눈이유는 속도가 너무빨라서 나눔
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

void AABCharacter::MultiShot() //멀티샷
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

void AABCharacter::RearShot() //후방사격
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

void AABCharacter::RightAndLeftShot() //좌우사격
{
	
	AArrow* Projectile4 = ArrowPooler->GetArrowPooledObject();
	if (Projectile4 != nullptr)
	{
		FVector Firepos4 = (DiagonalPos3->GetComponentLocation() - GetMesh()->GetComponentLocation());// 화살이 날아갈 방향에서 캐릭터 매시의 위치값을 빼서 날아갈 방향백터를 구한다.
		Firepos4.Z = 0;
		Projectile4->SetActive(true);
		Projectile4->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile4->ProjectileMovenent->Velocity = Firepos4 * Projectile4->ProjectileMovenent->InitialSpeed / 500;//방향백터와 속도값을 곱해서 속도를 구한다.(속도:방향과 속력이 같이있음) 500을 나눈이유는 속도가 너무빨라서 나눔
	}
	AArrow* Projectile5 = ArrowPooler->GetArrowPooledObject();
	if (Projectile5 != nullptr)
	{
		FVector Firepos5 = (DiagonalPos4->GetComponentLocation() - GetMesh()->GetComponentLocation());// 화살이 날아갈 방향에서 캐릭터 매시의 위치값을 빼서 날아갈 방향백터를 구한다.
		Firepos5.Z = 0;
		Projectile5->SetActive(true);
		Projectile5->SetActorLocation((GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 120.0f) + GetActorForwardVector() * 100));
		Projectile5->ProjectileMovenent->Velocity = Firepos5 * Projectile5->ProjectileMovenent->InitialSpeed / 500;//방향백터와 속도값을 곱해서 속도를 구한다.(속도:방향과 속력이 같이있음) 500을 나눈이유는 속도가 너무빨라서 나눔
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
	if (OtherComp->ComponentHasTag(FName("ExpBall"))) //경험치볼을 습득하면
	{
		CurrentEXP += ExpAmount; //경험치볼의 경험치량만큼 현재 캐릭터의 경험치를 상승시킨다.
		Cast<UABGameInstance>(GetGameInstance())->SetCurrentExp(CurrentEXP); //게임인스턴스에서의 현재 캐릭터 경험치를 상승시킨다.
		if (CurrentEXP == MaxEXP) //만약 현재경험치가 레벨당 최대경험치에 도달하면
		{
			CurrentEXP = 0; //현재경험치를 0으로만들고
			CurrentLevel++; //레벨을 1 올려준다.
			Cast<UABGameInstance>(GetGameInstance())->SetCurrentExp(CurrentEXP); //게임인스턴스의 현재경험치량을 캐릭터의 경험치량과 동기화한다.
			Cast<UABGameInstance>(GetGameInstance())->SetCurrentLevel(CurrentLevel); //게임인스턴스의 현재레벨을 캐릭터의 레벨과 동기화한다.
			Cast<UABGameInstance>(GetGameInstance())->SetCurrentMaxExp(CurrentLevel); //게임인스턴스에서 올라간 레벨에 맞게 최대경험치를 다시 셋팅한다.
			MaxEXP = Cast<UABGameInstance>(GetGameInstance())->InsCurrentMaxExp; //캐릭터의 최대경험치량을 레벨에 맞게 다시 셋팅한다.

		}
	}
	
}


