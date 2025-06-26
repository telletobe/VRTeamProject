// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameWidget.h"
#include <PlayerCharacter.h>
#include "Components/WidgetComponent.h"

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
		UWidgetComponent* Widget = Player->GetWidgetComponent();
		if (IsValid(Widget))
		{
			Widget->SetWidget(this);

			if (Widget->GetVisibleFlag() == false)
			{
				Widget->SetVisibility(true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetComp inValid"));
		}
	}
}