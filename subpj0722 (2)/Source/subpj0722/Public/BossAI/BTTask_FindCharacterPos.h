// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindCharacterPos.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API UBTTask_FindCharacterPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindCharacterPos();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
