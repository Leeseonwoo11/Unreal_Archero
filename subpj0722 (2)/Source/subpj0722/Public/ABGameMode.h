// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "ABGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SUBPJ0722_API AABGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
public:
	AABGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> PlayerEXPHUDClass;

	UPROPERTY()
	class UUserWidget* CurrentWidgt;
};
