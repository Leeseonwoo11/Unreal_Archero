// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_FlameAttack.h"
#include "ABBossAIController.h"
#include "ABBoss.h"
//원거리공격

UBTTaskNode_FlameAttack::UBTTaskNode_FlameAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_FlameAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ABBoss = Cast<AABBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (ABBoss == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	ABBoss->FlameAttack();
	return EBTNodeResult::InProgress;
}

void UBTTaskNode_FlameAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
