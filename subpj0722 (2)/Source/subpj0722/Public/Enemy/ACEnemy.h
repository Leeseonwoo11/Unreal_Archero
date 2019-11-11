// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "ABEnemy.h"
#include "ACEnemy.generated.h"

/**
 * ABEnemy를 상속받아 스켈레탈메시가 변경되고 원거리 공격하는 함수만 추가된다.
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
