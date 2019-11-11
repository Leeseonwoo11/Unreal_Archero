// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/GameInstance.h"
#include "ABEnemy.h"
#include "ABBoss.h"
#include "ABCharacter.h"
#include "TableManager.h"
#include "ABGameInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FFindEnmey_MapCall);
DECLARE_DELEGATE(FSpawnExpBall);

UCLASS()
class SUBPJ0722_API UABGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	virtual void Init() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Stage = 1;



	UPROPERTY(VisibleAnywhere)
	float CharCurrentHP = 100000;
	UPROPERTY(VisibleAnywhere)
	float CharMaxHp = 100000;
	UFUNCTION()
	void SetCurrentHP(float value);
	UFUNCTION()
	void SetMaxHP(float value);

	UPROPERTY(VisibleAnywhere)
		int32 InsCurrentExp = 0;
	UPROPERTY(VisibleAnywhere)
		int32 InsCurrentMaxExp;
	UPROPERTY(VisibleAnywhere)
		int32 InsCurrentLevel = 1;
	UPROPERTY(VisibleAnywhere)
		int32 MaxLevel;


	UFUNCTION(BlueprintCallable)
		void SetCurrentExp(int32 val);
	UFUNCTION(BlueprintCallable)
		void SetCurrentMaxExp(int32 CurrentLevel);
	UFUNCTION(BlueprintCallable)
		void SetCurrentLevel(int32 val);
	UFUNCTION(BlueprintCallable)
		int32 GetCurrentLevel();
	


	UPROPERTY(BlueprintReadOnly)
	AABEnemy* InsEnemy = nullptr;

	UFUNCTION()
	void SetEnemyptr(AABEnemy* Enemyptr);

	UPROPERTY(BlueprintReadOnly)
	AABEnemy* InsDeadEnmey = nullptr;

	UFUNCTION()
	void SetDeadEnemyptr(AABEnemy* Enemyptr);
	
	UFUNCTION()
	void CharacterCallToken();

	UPROPERTY(VisibleAnywhere)
	bool bGIDiagonal = false;
	UFUNCTION()
	void SetbGIDiagonal(bool val);

	UPROPERTY(VisibleAnywhere)
	bool bGIMulti = false;
	UFUNCTION()
	void SetbGIMulti(bool val);

	UPROPERTY(VisibleAnywhere)
	bool bGIRear = false;
	UFUNCTION()
	void SetbGIRear(bool val);

	UPROPERTY(VisibleAnywhere)
	bool bGIRightAndLeft = false;
	UFUNCTION()
	void SetbGIRightAndLeft(bool val);

	UPROPERTY(VisibleAnywhere)
	bool bGIPenetrate = false;
	UFUNCTION()
	void SetbGIPenetrate(bool val);

	UFUNCTION()
	void SpawnExpBallfunc();

	FFindEnmey_MapCall FindEnmey_MapCall;

	FSpawnExpBall SpawnExpBall;

	TableManager* TableMgr;

	AABBoss* BossPTR;

	UFUNCTION()
	void SetBossPTR(AABBoss* ptr);

	UFUNCTION(BlueprintCallable)
	void SetDefault();
};
