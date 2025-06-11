// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UMapSelectWidget::NativeConstruct()
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
	
}

void UMapSelectWidget::NativeDestruct()
{
	Super::NativeDestruct();

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }

}
