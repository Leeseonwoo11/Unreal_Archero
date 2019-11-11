// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABBossAIController.h"
#include "ABCharacter.h"
#include "ABBoss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) // 범위내에서 목표물 찾기(플레이어 캐릭터 찾기)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f; // 탐색 범위

	if (World == nullptr)
	{
		return;
	}
	TArray<FOverlapResult>OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel
	(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5, // 내 경우는 콜리전 트레이스채널을 찾아보니 5번이였음 
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	if (bResult) //무언가가 트레이스 채널에 걸렸을때
	{
		for (auto OverlapResult : OverlapResults) //트레이스채널 5번에 오버랩된 액터들 중에서
		{
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			if(ABCharacter && ABCharacter->GetController()->IsPlayerController())//ABCharacter가 있고 그 ABCharacter가 플레이어라면
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABBossAIController::TargetKey, ABCharacter);
				if (Cast<AABBoss>(ControllingPawn)->IsActive()) //캐릭터가 걸려들었을때 ABBoss가 활성화 되어있다면
				{
					DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2); // 파란 포인트 하나 찍어줌
					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // 구체 색깔 초록색으로 바꿔 그려줌
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f); // 파란색 줄하나 그려줌
				}
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABBossAIController::TargetKey, nullptr);// 위에서 리턴이 안되었다면 캐릭터를 찾지 못했으니 블랙보드의 타겟키를 널로 바꿔줌

	if (Cast<AABBoss>(ControllingPawn)->IsActive()) //AABBoss는 활성화 되어있지만 아무것도 안걸렸다면
	{
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); // 아무것도 안걸렸으면 빨간색 구체만 그려줌
	}
}
