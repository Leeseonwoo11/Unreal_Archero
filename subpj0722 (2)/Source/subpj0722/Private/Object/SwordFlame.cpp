// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlame.h"

ASwordFlame::ASwordFlame()
{
	ConstructorHelpers::FObjectFinder<UParticleSystem>SWORD_FLAME(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBall")); //파티클만 바꾼 매직볼
	if (SWORD_FLAME.Succeeded())
	{
		Particle->SetTemplate(SWORD_FLAME.Object);
	}
	Particle->SetWorldScale3D(FVector(10, 1, 10)); // 스케일 키워줌
}