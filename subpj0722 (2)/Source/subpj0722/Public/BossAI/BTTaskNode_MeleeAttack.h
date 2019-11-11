// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API UBTTaskNode_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_MeleeAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool IsMeleeAttacking = false;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
