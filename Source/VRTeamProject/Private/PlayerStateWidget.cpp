// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"


void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI Mode;
        Mode.SetHideCursorDuringCapture(false);
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        Mode.SetWidgetToFocus(TakeWidget());
        PC->SetInputMode(Mode);

    }

	if (btn_Quit)
	{
		btn_Quit->OnClicked.AddDynamic(this, &UPlayerStateWidget::GameQuit);
	}
    if (btn_Option)
    {
        btn_Option->OnClicked.AddDynamic(this, &UPlayerStateWidget::Option);
    }


}

void UPlayerStateWidget::NativeDestruct()
{
    Super::NativeDestruct();

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }
}


void UPlayerStateWidget::GameQuit()
{
}

void UPlayerStateWidget::Option()
{
}

void UPlayerStateWidget::UpdatePlayerStats()
{
}


