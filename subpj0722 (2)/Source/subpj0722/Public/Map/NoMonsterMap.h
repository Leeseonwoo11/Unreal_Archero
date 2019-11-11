// Fill out your copyright notice in the Description page of Project Settings.

//**********************************
//���� ���¸�  ������ƮǮ�� ������ ���� �ʾƼ� ������ �ȵ�
//���ʽ��� �������� ����� �� Max HP�� �÷��ְų� CurrentHP�� �÷��� �� �ִ� ����� �߰��� ��ȹ
//**********************************

#pragma once


#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "NoMonsterMap.generated.h"


UCLASS()
class SUBPJ0722_API ANoMonsterMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoMonsterMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		UStaticMeshComponent* Body;
};