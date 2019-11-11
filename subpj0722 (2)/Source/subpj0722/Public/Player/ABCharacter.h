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
	//       ������Ƽ ����
	//***********************************
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HPBar;
	UPROPERTY()
	AABBoss* Boss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AArrow> ArrowClass;

	//***********************************
	// �ڵ�Ÿ���ÿ� �ʿ��� ���� (���� ��ġ, �÷��̾���ġ, ����ȸ���� )
	//***********************************
	FVector DirectionToMove = FVector::ZeroVector; //ĳ���� ������ (0���� Ŀ���� �����̴� �������� ĳ���� ����)
	UPROPERTY(BlueprintReadWrite)
	FRotator EnemyRot = FRotator::ZeroRotator; //���� �������� ȸ��
	UPROPERTY(BlueprintReadWrite)
	FVector Charloc = FVector::ZeroVector;; // ĳ������ ��ġ
	UPROPERTY(BlueprintReadWrite)
	FVector Enemyloc = FVector::ZeroVector; //���� ��ġ
	UPROPERTY()
	FRotator WidgetRot; //������ ȸ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos1;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DiagonalPos4;


	//***********************************
	//  ĳ������ HP, ����ġ, ����
	//***********************************
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterHP)
	float CurrentHP; //�����ν��Ͻ����� �ҷ����� ���ϰ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterHP)
	float MaxHP;  //�����ν��Ͻ����� �ҷ����� ���ϰ� �Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 CurrentEXP; //�����ν��Ͻ����� �ҷ����� ���ϰ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 MaxEXP;  //�����ν��Ͻ����� �ҷ����� ���ϰ� �Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 CurrentLevel;  //�����ν��Ͻ����� �ҷ����� ���ϰ� �Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterExp)
	int32 MaxLevel;

	//***********************************
	//  �ӽü�������
	//***********************************
	UPROPERTY(EditAnywhere, Category = CharacterExp)
	int32 ExpAmount = 50; //�ӽü��� ����ġ ���� ����ġ���� ������ ������
	UPROPERTY(EditAnywhere, Category = EnemyWeaponDamage)
	float AxeDamage = 500; //�ӽü��� ���Ⱑ �������� ������ ������
	UPROPERTY(EditAnywhere, Category = EnemyWeaponDamage)
	float MagicBallDamage = 5000; //�ӽü��� ���Ⱑ �������� ������ ������

	//***********************************
	//  Ÿ�̸��ڵ�
	//***********************************
	UPROPERTY()
	FTimerHandle FireTimerHandler1; //��ã�⸦ ���� Ÿ�̸�
	UPROPERTY()
	FTimerHandle FireTimerHandler2; //�߻縦 ���� Ÿ�̸�
	UPROPERTY()
	FTimerHandle FireTimerHandler3; //��Ƽ�������� Ÿ�̸�
	UPROPERTY()
	FTimerHandle FireTimerHandler4; //����ã�⸦ ���� Ÿ�̸�
	UPROPERTY()
	FTimerHandle FireTimerHandler5; //�������� ȭ�� �߻縦 ���� Ÿ�̸�

	//***********************************
	//  ĳ���� ���� �Լ��� ������ (�߻� ������)
	//***********************************
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	UFUNCTION()
	void NoFire();  //�߻�Ұ���
	UFUNCTION()
	void Fire();	//�߻簡��
	UFUNCTION()
	void FindEnemy(); //��ã��
	UFUNCTION()
	void FindBoss(); // ����ã��
	UPROPERTY(EditAnywhere)
	float ShotDulation = 0.5f; //�߻簣��
	void Shot(); //����� �߻�
	void BossShot(); // ������ �߻�
	UPROPERTY()
	bool IsEnemyExist;  //���� ���翩�� true or false
	UPROPERTY(BlueprintReadOnly)
	bool IsBossExist; // ������ ���翩�� true or false
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool BPShotCheck = false; //�ִԺ������Ʈ���� ���ϸ��̼��� �����Ű�� ���ؼ� �����ϴ� ��  (ĳ������ ���� �ִϸ��̼�)
	UPROPERTY()
	UArrowPool* ArrowPooler;

	//***********************************
	//       ��ų��
	//***********************************
	UPROPERTY(EditAnywhere, Category = Shot)
	bool bDiagonal;
	void DiagonalShot(); //�缱��
	void SetDiagonalShot();

	UPROPERTY(EditAnywhere,Category = Shot)
	bool bMulti;
	void MultiShot(); //��Ƽ��
	void SetMultiShot();

	UPROPERTY(EditAnywhere, Category = Shot)
	bool bRear;
	void RearShot(); //�Ĺ漦
	void SetRearShot();

	UPROPERTY(EditAnywhere, Category = Shot)
	bool bRightAndLeft;
	void RightAndLeftShot(); //�¿켦
	void SetRightAndLeftShot();

	UPROPERTY(EditAnywhere, Category = Shot)
	bool bPenetrateShot;
	void PenetrateShot(AArrow* Arrowptr);//���뼦
	void SetPentrateShot();

	//***********************************
	//       �ݸ��� �������Լ�
	//***********************************
	UFUNCTION()
	void OnActorBeginOverlap2(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//***********************************
	//       ��������Ʈ
	//***********************************
	FNoEnemyInField NoEnemyInField;
};
