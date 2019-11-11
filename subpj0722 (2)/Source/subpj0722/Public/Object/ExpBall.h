// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ExpBall.generated.h"

/*
//*****************************
// �Ϲ� ���� ������ ����� ����ġ ��
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
	bool IsAbsorb = false; //ĳ���Ϳ� ������ɿ��� �������Ʈ���� �����ؼ� ����Ѵ�.

	UPROPERTY()
	bool Active = false;

	UFUNCTION()
	void SetActive(bool InActive); //Ȱ�� �Ǵ� ��Ȱ�� ��

	UFUNCTION(BlueprintCallable)
	void Deactivate(); //����ġ�� ��Ƽ��

	UFUNCTION()
	bool IsActive(); //����ġ�� Ȱ��ȭ ���� ����
};
