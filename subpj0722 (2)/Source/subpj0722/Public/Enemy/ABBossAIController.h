// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "ABBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API AABBossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AABBossAIController();
	virtual void OnPossess(APawn* InPawn) override;
	static const FName HomePosKey; //스폰된 위치 키
	static const FName CharacterPosKey; // 캐릭터의 위치 키 
	static const FName TargetKey; // 타겟(움직일곳)의 위치 키

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset; // 비헤이비어 트리
	UPROPERTY()
	class UBlackboardData* BBAsset;// 블랙보드
};
