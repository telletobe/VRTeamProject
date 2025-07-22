// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "EndGameWidget.h"
#include <PlayerCharacter.h>
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include <VRProjectGameModeBase.h>
#include <Kismet/KismetSystemLibrary.h>

void UEndGameWidget::NativeConstruct()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		if (IsValid(Player))
		{
			Player->OnPlayerDeath.AddUniqueDynamic(this,&UEndGameWidget::ShowEndGame);
		}
	}
}

void UEndGameWidget::ShowEndGame() 
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		UWidgetComponent* WidgetComp = Player->GetWidgetComponent();
		if (IsValid(WidgetComp))
		{
			WidgetComp->SetWidget(this);			
			if (WidgetComp->GetVisibleFlag() == false)
			{
				WidgetComp->SetVisibility(true);
				Player->SetVisibleRazerMesh(true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetComp inValid"));
		}
	}
}
