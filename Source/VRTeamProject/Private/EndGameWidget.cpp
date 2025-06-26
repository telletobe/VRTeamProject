// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameWidget.h"
#include <PlayerCharacter.h>
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include <VRProjectGameModeBase.h>

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

	if (BT_RestartButton)
	{
		BT_RestartButton->OnClicked.AddUniqueDynamic(this, &UEndGameWidget::ReStart);
	}

	if (BT_QuitButton)
	{
		BT_QuitButton->OnClicked.AddUniqueDynamic(this, &UEndGameWidget::Quit);
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
			////////////////////
			#if WITH_EDITOR
			AddToViewport();
			PC->SetShowMouseCursor(true);
			#endif
			/////////////////////
			
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

void UEndGameWidget::ReStart()
{
	AVRProjectGameModeBase* GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->TriggerGameReStart();
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("GameMode invalid"));
	}
	////////////////////////
	#if WITH_EDITOR
		RemoveFromParent();
	#endif
	/////////////////////////
		if (APlayerController* PC = GetOwningPlayer())
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
			UWidgetComponent* Widget = Player->GetWidgetComponent();
			if (Widget)
			{
				Widget->SetVisibility(false);
			}
		}
}

void UEndGameWidget::Quit()
{

}