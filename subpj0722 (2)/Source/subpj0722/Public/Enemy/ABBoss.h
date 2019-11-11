// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ABBoss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRushAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMeleeAttackEndDelegate);

UCLASS()
class SUBPJ0722_API AABBoss : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	//****************************
	//���� ����
	//****************************
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsRush = false;	//����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsMelee = false;	//��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsFlame = false;	//���Ÿ� �˱� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDead = false;	//�׾��� ��ҳ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BossHP;

	//****************************
	//���� ��ų(���ݱ��)
	//****************************
	UFUNCTION()
	void RushAttack();
	UFUNCTION(BlueprintCallable)
	void RushAttackEnd();
	UFUNCTION()
	void MeleeAttack();
	UFUNCTION(BlueprintCallable)
	void MeleeAttackEnd();
	UFUNCTION()
	void FlameAttack();

	//****************************
	//���� Ȱ��ȭ ��Ȱ��ȭ ����
	//������ ĳ���� ó�ٺ��� ����
	//****************************
	UPROPERTY()
	bool Active = false;
	UFUNCTION()
	void SetActive(bool InActive);
	UFUNCTION(BlueprintCallable)
	void Deactivate();
	UFUNCTION(BlueprintCallable)
	bool IsActive();
	void LookCharacter(); //ĳ���� ó�ٺ���(���� ȸ��)

	//****************************
	//ĸ���ݸ��� ������
	//****************************
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//****************************
	//��������Ʈ
	//****************************
	FOnRushAttackEndDelegate OnRushAttackEnd;
	FOnMeleeAttackEndDelegate OnMeleeAttackEnd;
};
