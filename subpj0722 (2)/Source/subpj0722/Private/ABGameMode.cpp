// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"

void AABGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidgt = CreateWidget<UUserWidget>(GetWorld(), PlayerEXPHUDClass);
	if (CurrentWidgt != nullptr)
	{
		CurrentWidgt->AddToViewport();
	}


}
AABGameMode::AABGameMode()
{
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
	
	 //캐릭터의 경험치 바 셋
	ConstructorHelpers::FClassFinder<UUserWidget>UW_EXPBAR(TEXT("/Game/Mynew/CharacterHUD/BP_Character_EXPbar"));
	if (UW_EXPBAR.Succeeded())
	{
		PlayerEXPHUDClass = UW_EXPBAR.Class;
	}

}
