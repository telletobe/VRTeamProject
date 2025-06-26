// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "PlayerCharacter.h"
#include "VRProjectGameModeBase.h"


void UPlayerStateWidget::ReSetHp()
{
	if (HPBar)
	{
		HPBar->SetPercent(1.0f);
	}
}


void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		if (Player)
		{
			Player->OnHealthChange.AddUniqueDynamic(this,&UPlayerStateWidget::UpdatePlayerHP);
		}
	}	

	if (AVRProjectGameModeBase* GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnRestart.AddUniqueDynamic(this, &UPlayerStateWidget::ReSetHp);
	}
	
}

void UPlayerStateWidget::UpdatePlayerHP(float CurrentHp, float MaxHp)
{
	if (HPBar && MaxHp > 0.0f)
	{
		HPBar->SetPercent(CurrentHp/MaxHp);
	}
	//Set Text.
}

void UPlayerStateWidget::UpdatePlayerDef()
{
	if (DefBar)
	{
		DefBar->SetPercent(1.0f);
	}
	//Set Text
}

void UPlayerStateWidget::UpdatePlayerExp(float CurrentExp)
{
	if (ExpBar)
	{
		ExpBar->SetPercent(0.0f);
	}
		//Set Text
}
