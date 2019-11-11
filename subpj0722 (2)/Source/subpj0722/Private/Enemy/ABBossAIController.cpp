// Fill out your copyright notice in the Description page of Project Settings.


#include "ABBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AABBossAIController::HomePosKey(TEXT("HomePos"));
const FName AABBossAIController::CharacterPosKey(TEXT("CharacterPos"));
const FName AABBossAIController::TargetKey(TEXT("Target"));

AABBossAIController::AABBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Mynew/AI/BB_ABBoss")); //블랙보드 불러서 셋
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Mynew/AI/BT_ABBoss")); //비헤이비어트리 불러서 셋
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AABBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Error, TEXT("AIController couldnt run behavior tree"));
		}
	}
}
