// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_MeleeAttackRange.h"
#include "ABCharacter.h"
#include "ABBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_MeleeAttackRange::UBTDecorator_MeleeAttackRange()
{
	NodeName = TEXT("CanMeleeAttack");
}

bool UBTDecorator_MeleeAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); //컨트롤하는 폰 ABBoss
	if (ControllingPawn == nullptr)
	{
		return false;
	}
	auto Target = Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABBossAIController::TargetKey)); //타겟은 플레이어 캐릭터
	if (Target == nullptr)
	{
		return false;
	}
	bResult = (Target->GetDistanceTo(ControllingPawn) <= MeleeAttackRange);// 근접공격의 범위내에 있다면 결과를 true;
	return bResult; //근접공격 한다 또는 안한다 리턴
}
