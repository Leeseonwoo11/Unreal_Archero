// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindCharacterPos.h"
#include "ABBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "ABCharacter.h"



UBTTask_FindCharacterPos::UBTTask_FindCharacterPos()
{
	NodeName = TEXT("FindCharacterPos");
}


EBTNodeResult::Type UBTTask_FindCharacterPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // ��Ʈ�Ѹ����� �޾ƿ´�.
	if (ControllingPawn == nullptr) //��Ʈ�Ѹ� ���� ���̶�� 
	{
		UE_LOG(LogTemp, Error, TEXT("ControllingPawn is nullptr"));
		return EBTNodeResult::Failed; // failed�����Ѵ�.
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); // �׺�ý����� �޾ƿ´�.
	if (NavSystem == nullptr) // �׺�ý����� ���̶��
	{
		UE_LOG(LogTemp, Error, TEXT("Navsystem is nullptr"));
		return EBTNodeResult::Failed; // faliled �����Ѵ�.
	}

	FNavLocation NextCharacterPos; // ������ �������� �̵��� ��� ��ġ
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); // ĳ���� �����͸� �޾ƿ´�.
	if (temp!=nullptr) // ĳ���� �����Ͱ� ���� �ƴ϶��
	{
		NextCharacterPos.Location = temp->GetActorLocation(); //location�� �ʱ�ȭ ���ش�.
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABBossAIController::CharacterPosKey, NextCharacterPos.Location); //AIController�� Ű���� �ٲ��ش�.
		return EBTNodeResult::Succeeded; //Succeeded����
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("temp is nullptr"));

		return EBTNodeResult::Failed; //failed����
	}

}
