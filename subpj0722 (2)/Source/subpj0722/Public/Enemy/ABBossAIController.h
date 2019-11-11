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
	static const FName HomePosKey; //������ ��ġ Ű
	static const FName CharacterPosKey; // ĳ������ ��ġ Ű 
	static const FName TargetKey; // Ÿ��(�����ϰ�)�� ��ġ Ű

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset; // �����̺�� Ʈ��
	UPROPERTY()
	class UBlackboardData* BBAsset;// ������
};
