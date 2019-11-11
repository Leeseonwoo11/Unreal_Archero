// Fill out your copyright notice in the Description page of Project Settings.
// *****************
// 캐릭터의 레벨과 경험치의 데이터 테이블 형식
// *****************
#pragma once

#include "EngineMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "LevelDataTables.generated.h"

USTRUCT(BlueprintType)
struct FLevelDataTableRow: public FTableRowBase
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Level")
	int32 nLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Level")
	int32 nMaxExp;
};

UCLASS()
class SUBPJ0722_API ALevelDataTables : public AActor
{
	GENERATED_BODY()
};