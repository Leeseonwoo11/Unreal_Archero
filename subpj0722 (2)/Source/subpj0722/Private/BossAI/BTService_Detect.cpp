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

void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) // ���������� ��ǥ�� ã��(�÷��̾� ĳ���� ã��)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f; // Ž�� ����

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
		ECollisionChannel::ECC_GameTraceChannel5, // �� ���� �ݸ��� Ʈ���̽�ä���� ã�ƺ��� 5���̿��� 
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	if (bResult) //���𰡰� Ʈ���̽� ä�ο� �ɷ�����
	{
		for (auto OverlapResult : OverlapResults) //Ʈ���̽�ä�� 5���� �������� ���͵� �߿���
		{
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			if(ABCharacter && ABCharacter->GetController()->IsPlayerController())//ABCharacter�� �ְ� �� ABCharacter�� �÷��̾���
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABBossAIController::TargetKey, ABCharacter);
				if (Cast<AABBoss>(ControllingPawn)->IsActive()) //ĳ���Ͱ� �ɷ�������� ABBoss�� Ȱ��ȭ �Ǿ��ִٸ�
				{
					DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2); // �Ķ� ����Ʈ �ϳ� �����
					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // ��ü ���� �ʷϻ����� �ٲ� �׷���
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f); // �Ķ��� ���ϳ� �׷���
				}
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABBossAIController::TargetKey, nullptr);// ������ ������ �ȵǾ��ٸ� ĳ���͸� ã�� �������� �������� Ÿ��Ű�� �η� �ٲ���

	if (Cast<AABBoss>(ControllingPawn)->IsActive()) //AABBoss�� Ȱ��ȭ �Ǿ������� �ƹ��͵� �Ȱɷȴٸ�
	{
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); // �ƹ��͵� �Ȱɷ����� ������ ��ü�� �׷���
	}
}
