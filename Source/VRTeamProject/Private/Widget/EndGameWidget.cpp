// Fill out your copyright notice in the Description page of Project Settings.
#include "Widget/EndGameWidget.h"
#include "Player/PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include "System/VRProjectGameModeBase.h"
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
				WidgetComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				Player->SetVisibleRazerMesh(true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetComp inValid"));
		}
	}
}
