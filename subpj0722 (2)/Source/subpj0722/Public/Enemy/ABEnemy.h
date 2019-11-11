// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ABEnemy.generated.h"



UCLASS()
class SUBPJ0722_API AABEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABEnemy();
	bool DoOnce = true; //한번만 실행하기 위해서 만듬

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool Active;
	void Deactivate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetActive(bool InActive);
	bool IsActive();
	UPROPERTY()
	FVector Characterloc; // 캐릭터(player)의 위치
	UPROPERTY()
	FVector	Enemyloc;  // 현재 적(this)의 위치
	UPROPERTY()
	FRotator CharacterRot; //캐릭터를 바라보기위한 회전값
	int32 ticktime = 0; //destroy하려고 만듬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = HIT)
	bool Ishit; //맞았나 안맞았나 체크, 애니메이션에서 사용
	UFUNCTION()
	void OnActorBeginOverlap1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void LookCharacter(); //플레이어 캐릭터 처다보기
};
