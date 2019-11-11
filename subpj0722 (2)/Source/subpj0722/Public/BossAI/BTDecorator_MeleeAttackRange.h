// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_MeleeAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API UBTDecorator_MeleeAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_MeleeAttackRange();
	UPROPERTY(EditAnywhere)
	float MeleeAttackRange = 200.0f;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
