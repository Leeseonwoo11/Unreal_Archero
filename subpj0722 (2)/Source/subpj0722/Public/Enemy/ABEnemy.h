// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "ABEnemy.generated.h"



UCLASS()
class SUBPJ0722_API AABEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABEnemy();
	bool DoOnce = true; //�ѹ��� �����ϱ� ���ؼ� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool Active;
	void Deactivate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetActive(bool InActive);
	bool IsActive();
	UPROPERTY()
	FVector Characterloc; // ĳ����(player)�� ��ġ
	UPROPERTY()
	FVector	Enemyloc;  // ���� ��(this)�� ��ġ
	UPROPERTY()
	FRotator CharacterRot; //ĳ���͸� �ٶ󺸱����� ȸ����
	int32 ticktime = 0; //destroy�Ϸ��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = HIT)
	bool Ishit; //�¾ҳ� �ȸ¾ҳ� üũ, �ִϸ��̼ǿ��� ���
	UFUNCTION()
	void OnActorBeginOverlap1(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void LookCharacter(); //�÷��̾� ĳ���� ó�ٺ���
};
