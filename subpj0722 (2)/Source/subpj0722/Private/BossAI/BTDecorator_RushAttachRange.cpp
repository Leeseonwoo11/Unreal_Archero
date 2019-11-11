// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_RushAttachRange.h"
#include "ABBossAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_RushAttachRange::UBTDecorator_RushAttachRange()
{
	NodeName = TEXT("CanRuchAttack");
}

bool UBTDecorator_RushAttachRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); //��Ʈ�Ѹ����� AABBoss
	if (ControllingPawn == nullptr)
	{
		return false;
	}
	auto Target = Cast<AABCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AABBossAIController::TargetKey));;
	if (Target == nullptr)
	{
		return false;
	}

	bResult = (Target->GetDistanceTo(ControllingPawn) >= RushAttackRange);  //�������ݹ��� ���� �ִٸ� �������� true ���� �ƴϸ� �������� false ����
	return bResult;
}
