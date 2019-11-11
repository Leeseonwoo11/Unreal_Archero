// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MeleeAttack.h"
#include "ABBossAIController.h"
#include "ABBoss.h"

//근접공격

UBTTaskNode_MeleeAttack::UBTTaskNode_MeleeAttack()
{
	bNotifyTick = true;
	IsMeleeAttacking = false;
}

EBTNodeResult::Type UBTTaskNode_MeleeAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ABBoss = Cast<AABBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (ABBoss == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	ABBoss->MeleeAttack();
	IsMeleeAttacking = true;	
	ABBoss->OnMeleeAttackEnd.AddLambda([this]()->void {IsMeleeAttacking = false; });
	return EBTNodeResult::InProgress;
}

void UBTTaskNode_MeleeAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsMeleeAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
