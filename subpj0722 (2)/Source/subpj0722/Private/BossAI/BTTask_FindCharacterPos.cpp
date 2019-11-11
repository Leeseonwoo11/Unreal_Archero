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
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // 컨트롤링폰을 받아온다.
	if (ControllingPawn == nullptr) //컨트롤링 폰이 널이라면 
	{
		UE_LOG(LogTemp, Error, TEXT("ControllingPawn is nullptr"));
		return EBTNodeResult::Failed; // failed리턴한다.
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld()); // 네비시스템을 받아온다.
	if (NavSystem == nullptr) // 네비시스템이 널이라면
	{
		UE_LOG(LogTemp, Error, TEXT("Navsystem is nullptr"));
		return EBTNodeResult::Failed; // faliled 리턴한다.
	}

	FNavLocation NextCharacterPos; // 보스가 다음으로 이동할 장소 위치
	AABCharacter* temp = Cast<AABCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); // 캐릭터 포인터를 받아온다.
	if (temp!=nullptr) // 캐릭터 포인터가 널이 아니라면
	{
		NextCharacterPos.Location = temp->GetActorLocation(); //location을 초기화 해준다.
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABBossAIController::CharacterPosKey, NextCharacterPos.Location); //AIController의 키값을 바꿔준다.
		return EBTNodeResult::Succeeded; //Succeeded리턴
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("temp is nullptr"));

		return EBTNodeResult::Failed; //failed리턴
	}

}
