// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RushAttachRange.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API UBTDecorator_RushAttachRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_RushAttachRange();
	UPROPERTY(EditAnywhere)
	float RushAttackRange = 300.0f;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
