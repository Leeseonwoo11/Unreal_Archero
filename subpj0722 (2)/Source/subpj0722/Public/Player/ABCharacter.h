// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Arrow.h"
#include "ArrowPool.h"
#include "ABBoss.h"
#include "Runtime/Renderer/Public/MeshDrawShaderBindings.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FNoEnemyInField);

UCLASS()
class SUBPJ0722_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AABCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetControlMode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//***********************************
	//           Camera
	//***********************************
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	//***********************************
	//       프로퍼티 정의
	//***********************************
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HPBar;
	UPROPERTY()
	AABBoss* Boss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AArrow> ArrowClass;

	//***********************************
	// 자동타겟팅에 필요한 값들 (적의 위치, 플레이어위치, 위젯회전값 )
	//***********************************
	FVector DirectionToMove = FVector::ZeroVector; //캐릭터 움직임 (0보다 커지면 움직이는 방향으로 캐릭터 돌림)
	UPROPERTY(BlueprintReadWrite)
	FRotator EnemyRot = FRotator::ZeroRotator; //적을 보기위한 회전
	UPROPERTY(BlueprintReadWrite)
	FVector Charloc = FVector::ZeroVector;; // 캐릭터의 위치
	UPROPERTY(BlueprintReadWrite)
	FVector Enemyloc = FVector::ZeroVector; //적의 위치
	UPROPERTY()
	FRotator WidgetRot; //위젯의 회전정도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos1;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos4;


	//***********************************
	//  캐릭터의 HP, 경험치, 레벨
	//***********************************
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterHP)
	float CurrentHP; //게임인스턴스에서 불러오고 셋하고
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterHP)
	float MaxHP;  //게임인스턴스에서 불러오고 셋하고 한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 CurrentEXP; //게임인스턴스에서 불러오고 셋하고
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 MaxEXP;  //게임인스턴스에서 불러오고 셋하고 한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 CurrentLevel;  //게임인스턴스에서 불러오고 셋하고 한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 MaxLevel;

	//***********************************
	//  임시설정값들
	//***********************************
	UPROPERTY(EditAnywhere, Category = CharacterExp)
	int32 ExpAmount = 50; //임시설정 경험치 볼이 경험치량을 가지고 있을것
	UPROPERTY(EditAnywhere, Category = EnemyWeaponDamage)
	float AxeDamage = 500; //임시설정 무기가 데미지를 가지고 있을것
	UPROPERTY(EditAnywhere, Category = EnemyWeaponDamage)
	float MagicBallDamage = 5000; //임시설정 무기가 데미지를 가지고 있을것

	//***********************************
	//  타이머핸들
	//***********************************
	UPROPERTY()
	FTimerHandle FireTimerHandler1; //적찾기를 위한 타이머
	UPROPERTY()
	FTimerHandle FireTimerHandler2; //발사를 위한 타이머
	UPROPERTY()
	FTimerHandle FireTimerHandler3; //멀티샷을위한 타이머
	UPROPERTY()
	FTimerHandle FireTimerHandler4; //보스찾기를 위한 타이머
	UPROPERTY()
	FTimerHandle FireTimerHandler5; //보스에게 화살 발사를 위한 타이머

	//***********************************
	//  캐릭터 동작 함수와 설정값 (발사 움직임)
	//***********************************
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	UFUNCTION()
	void NoFire();  //발사불가능
	UFUNCTION()
	void Fire();	//발사가능
	UFUNCTION()
	void FindEnemy(); //적찾기
	UFUNCTION()
	void FindBoss(); // 보스찾기
	UPROPERTY(EditAnywhere)
	float ShotDulation = 0.5f; //발사간격
	void Shot(); //보통몹 발사
	void BossShot(); // 보스몹 발사
	UPROPERTY()
	bool IsEnemyExist;  //적의 존재여부 true or false
	UPROPERTY(BlueprintReadOnly)
	bool IsBossExist; // 보스의 존재여부 true or false
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool BPShotCheck = false; //애님블루프린트에서 에니매이션을 재생시키기 위해서 감시하는 값  (캐릭터의 동작 애니메이션)
	UPROPERTY()
	UArrowPool* ArrowPooler;

	//***********************************
	//       스킬들
	//***********************************
	UPROPERTY(EditAnywhere, Category = Shot)
	bool bDiagonal;
	void DiagonalShot(); //사선샷
	void SetDiagonalShot();

	UPROPERTY(EditAnywhere,Category = Shot)
	bool bMulti;
	void MultiShot(); //멀티샷
	void SetMultiShot();

	UPROPERTY(EditAnywhere, Category = Shot)
	bool bRear;
	void RearShot(); //후방샷
	void SetRearShot();

	UPROPERTY(EditAnywhere, Category = Shot)
	bool bRightAndLeft;
	void RightAndLeftShot(); //좌우샷
	void SetRightAndLeftShot();

	UPROPERTY(EditAnywhere, Category = Shot)
	bool bPenetrateShot;
	void PenetrateShot(AArrow* Arrowptr);//관통샷
	void SetPentrateShot();

	//***********************************
	//       콜리젼 오버랩함수
	//***********************************
	UFUNCTION()
	void OnActorBeginOverlap2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//***********************************
	//       델리게이트
	//***********************************
	FNoEnemyInField NoEnemyInField;
};
