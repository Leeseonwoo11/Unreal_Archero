// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "ABEnemy.h"
#include "ACEnemy.generated.h"

/**
 * ABEnemy�� ��ӹ޾� ���̷�Ż�޽ð� ����ǰ� ���Ÿ� �����ϴ� �Լ��� �߰��ȴ�.
 */
UCLASS()
class SUBPJ0722_API AACEnemy : public AABEnemy
{
	GENERATED_BODY()
public:
	AACEnemy();


	UFUNCTION(BlueprintCallable)
	void MagicBallFire();

};
