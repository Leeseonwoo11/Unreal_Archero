// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_RushAttack.h"
#include "ABBossAIController.h"
#include "ABBoss.h"

//돌격공격

UBTTaskNode_RushAttack::UBTTaskNode_RushAttack()
{
	bNotifyTick = true;
	IsRushAttacking = false;
}

EBTNodeResult::Type UBTTaskNode_RushAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ABBoss = Cast<AABBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (ABBoss == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	ABBoss->RushAttack();
	IsRushAttacking = true;
	ABBoss->OnRushAttackEnd.AddLambda([this]()->void {IsRushAttacking = false; });
	return EBTNodeResult::InProgress;
}

void UBTTaskNode_RushAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsRushAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	} 
}
