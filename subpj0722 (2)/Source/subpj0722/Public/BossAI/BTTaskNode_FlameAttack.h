// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FlameAttack.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API UBTTaskNode_FlameAttack : public UBTTaskNode
{
	GENERATED_BODY()
		
public:
	UBTTaskNode_FlameAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
