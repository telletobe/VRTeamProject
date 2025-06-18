// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"
#include "PlayerCharacter.h"


void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UPlayerStateWidget::SetHealthPercent(float Percent)
{
	if (HealthBar) HealthBar->SetPercent(Percent);
}

void UPlayerStateWidget::SetDefensePercent(float Percent)
{
	if (DefenseBar) DefenseBar->SetPercent(Percent);
}


void UPlayerStateWidget::UpdatePlayerStats()
{

}


