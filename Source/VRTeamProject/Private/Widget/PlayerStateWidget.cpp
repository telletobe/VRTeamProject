// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerStateWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Player/PlayerCharacter.h"
#include "System/VRProjectGameModeBase.h"

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
}

void UPlayerStateWidget::ReSetHp() //Restart ��ư�� �������� Ȱ��
{
	if (HPBar)
	{
		HPBar->SetPercent(1.0f);
	}
}


void UPlayerStateWidget::UpdatePlayerHP(float CurrentHp, float MaxHp)
{
	if (HPBar && MaxHp > 0.0f)
	{
		HPBar->SetPercent(CurrentHp/MaxHp);
	}

}

void UPlayerStateWidget::UpdatePlayerDef()
{
	if (DefBar)
	{
		DefBar->SetPercent(1.0f);
	}

}

void UPlayerStateWidget::UpdatePlayerExp(float CurrentExp)
{
	if (ExpBar)
	{
		ExpBar->SetPercent(0.0f);
	}

}
