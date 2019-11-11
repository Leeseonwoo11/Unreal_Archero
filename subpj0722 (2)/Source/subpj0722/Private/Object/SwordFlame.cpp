// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordFlame.h"

ASwordFlame::ASwordFlame()
{
	ConstructorHelpers::FObjectFinder<UParticleSystem>SWORD_FLAME(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBall")); //��ƼŬ�� �ٲ� ������
	if (SWORD_FLAME.Succeeded())
	{
		Particle->SetTemplate(SWORD_FLAME.Object);
	}
	Particle->SetWorldScale3D(FVector(10, 1, 10)); // ������ Ű����
}