// Fill out your copyright notice in the Description page of Project Settings.


#include "VRProjectGameModeBase.h"
#include "PlayerCharacter.h"

AVRProjectGameModeBase::AVRProjectGameModeBase()
{

	DefaultPawnClass = APlayerCharacter::StaticClass();
	
}

void AVRProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}
