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
			////////////////////
			#if WITH_EDITOR
			AddToViewport();
			PC->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TakeWidget()); // ���� ������ ��Ŀ�� ����
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
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
