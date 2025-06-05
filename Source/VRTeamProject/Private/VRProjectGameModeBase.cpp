// Fill out your copyright notice in the Description page of Project Settings.


#include "VRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"

AVRProjectGameModeBase::AVRProjectGameModeBase()
{

	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	
}

void AVRProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}
