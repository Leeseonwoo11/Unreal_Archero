// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ExpBall.generated.h"

/*
//*****************************
// 일반 몹이 죽으면 생기는 경험치 볼
//*****************************
*/


UCLASS()
class SUBPJ0722_API AExpBall : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExpBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void Init() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AbsorbInCharacter();

	UPROPERTY(BlueprintReadWrite)
	bool IsAbsorb = false; //캐릭터에 흡수가능여부 블루프린트에서 참조해서 사용한다.

	UPROPERTY()
	bool Active = false;

	UFUNCTION()
	void SetActive(bool InActive); //활성 또는 비활성 셋

	UFUNCTION(BlueprintCallable)
	void Deactivate(); //경험치볼 디엑티브

	UFUNCTION()
	bool IsActive(); //경험치볼 활성화 여부 리턴
};
