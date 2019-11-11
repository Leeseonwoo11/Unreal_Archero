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
	//보스 상태
	//****************************
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsRush = false;	//돌진
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsMelee = false;	//근접공격
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsFlame = false;	//원거리 검기 공격
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDead = false;	//죽엇나 살았나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BossHP;

	//****************************
	//보스 스킬(공격기능)
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
	//보스 활성화 비활성화 여부
	//보스가 캐릭터 처다보기 등등등
	//****************************
	UPROPERTY()
	bool Active = false;
	UFUNCTION()
	void SetActive(bool InActive);
	UFUNCTION(BlueprintCallable)
	void Deactivate();
	UFUNCTION(BlueprintCallable)
	bool IsActive();
	void LookCharacter(); //캐릭터 처다보기(보스 회전)

	//****************************
	//캡슐콜리젼 오버랩
	//****************************
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//****************************
	//델리게이트
	//****************************
	FOnRushAttackEndDelegate OnRushAttackEnd;
	FOnMeleeAttackEndDelegate OnMeleeAttackEnd;
};
