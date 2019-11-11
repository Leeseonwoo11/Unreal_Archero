// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "MacicBall.h"
#include "SwordFlame.generated.h"


//********************************************
// 보스와 캐릭터 사이가 멀때 보스가 던지는 검기
// 원거리 몬스터가 던지는 매직볼을 상속받아 씀
//********************************************

UCLASS()
class SUBPJ0722_API ASwordFlame : public AMacicBall
{
	GENERATED_BODY()
public:
	ASwordFlame();
	
};
