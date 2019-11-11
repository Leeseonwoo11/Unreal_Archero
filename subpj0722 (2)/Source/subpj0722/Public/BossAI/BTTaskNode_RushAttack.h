// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_RushAttack.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API UBTTaskNode_RushAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_RushAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool IsRushAttacking = false;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
