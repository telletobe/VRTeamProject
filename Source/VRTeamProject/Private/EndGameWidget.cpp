// Fill out your copyright notice in the Description page of Project Settings.


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
	UE_LOG(LogTemp, Warning, TEXT("Call ShowEndGame"));
	if (APlayerController* PC = GetOwningPlayer())
	{
		UE_LOG(LogTemp,Warning,TEXT("Call ShowEndGame"));
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		UWidgetComponent* WidgetComp = Player->GetWidgetComponent();
		if (IsValid(WidgetComp))
		{
			WidgetComp->SetWidget(this);
			////////////////////
			#if WITH_EDITOR
			AddToViewport();
			PC->SetShowMouseCursor(true);
			#endif
			/////////////////////
			
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
	if (APlayerController* PC = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
	}
	else
	{
		//컨트롤러 정보가 없을 시 비정상 강제종료.
		FGenericPlatformMisc::RequestExit(false);
	}
}