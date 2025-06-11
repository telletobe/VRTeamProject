// Fill out your copyright notice in the Description page of Project Settings.


#include "VRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"

AVRProjectGameModeBase::AVRProjectGameModeBase()
{

	bIsClear = false;

	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	
}

void AVRProjectGameModeBase::TriggerGameClear()
{

	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::MakeRandomColor(), TEXT("Clear Game"));
	bIsClear = true;
	return;
}

void AVRProjectGameModeBase::TriggerGameStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("Start Game"));
	bIsClear = false;
	return;
}

void AVRProjectGameModeBase::ChangePlayerAliveState()
{
	bPlayerAlive = !bPlayerAlive;
	UE_LOG(LogTemp,Warning,TEXT("PlayerAlive : %s"), bPlayerAlive ? TEXT("true") : TEXT("false"));
	return;
}


void AVRProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		Player->OnPlayerDeath.AddDynamic(this, &AVRProjectGameModeBase::ChangePlayerAliveState);
	}

	
}
