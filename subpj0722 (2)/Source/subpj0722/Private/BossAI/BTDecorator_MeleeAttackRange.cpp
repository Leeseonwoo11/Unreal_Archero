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
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); //��Ʈ���ϴ� �� ABBoss
	if (ControllingPawn == nullptr)
	{
		return false;
	}
	auto Target = Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABBossAIController::TargetKey)); //Ÿ���� �÷��̾� ĳ����
	if (Target == nullptr)
	{
		return false;
	}
	bResult = (Target->GetDistanceTo(ControllingPawn) <= MeleeAttackRange);// ���������� �������� �ִٸ� ����� true;
	return bResult; //�������� �Ѵ� �Ǵ� ���Ѵ� ����
}
