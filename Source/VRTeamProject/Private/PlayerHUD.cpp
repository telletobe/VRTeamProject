// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "MapselectWidget.h"
#include "PlayerStateWidget.h"
#include "Blueprint/UserWidget.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UMapSelectWidget> Selectwidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/MapUI/WBP_MapSelectWidget.WBP_MapSelectWidget_C'"));

	if (Selectwidget.Succeeded())
	{
		MapSelect = Selectwidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UPlayerStateWidget> PlayerStateWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/WBP_PlayerStateWidget.WBP_PlayerStateWidget_C'"));

	if (PlayerStateWidget.Succeeded())
	{
		PlayerState = PlayerStateWidget.Class;
	}


}


void APlayerHUD::BeginPlay()
{

	if (PC == nullptr)
	{
		PC = GetOwningPlayerController();
	}

	if (MapSelect && !MapSelectInstance)
	{
		MapSelectInstance = CreateWidget<UMapSelectWidget>(GetWorld(), MapSelect);
	}

	if (PlayerState && !PlayerStateInstance)
	{
		PlayerStateInstance = CreateWidget<UPlayerStateWidget>(GetWorld(), PlayerState);
	}

	Mode.SetHideCursorDuringCapture(false);
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

}

void APlayerHUD::ToggleMapSelect()
{
	
	if (IsValid(MapSelectInstance))
	{
		if (MapSelectInstance->IsInViewport())
		{
			MapSelectInstance->RemoveFromParent();      // 커서 OFF
			PC->bShowMouseCursor = false;
			if (PC) PC->SetInputMode(FInputModeGameOnly());
		}
		else
		{
			if (PlayerStateInstance->IsInViewport())
			{
				PlayerStateInstance->RemoveFromParent();
			}
			MapSelectInstance->AddToViewport();      // 커서 ON
			PC->bShowMouseCursor = true;
			if (PC) PC->SetInputMode(Mode);
		}
	}
}

void APlayerHUD::PlayerStateShow()
{
	if (IsValid(PlayerStateInstance))
	{
		if (GetWorld()->GetMapName().Contains("Map"))
		{
			if (PlayerStateInstance->IsInViewport())
			{
				PlayerStateInstance->RemoveFromParent();
				PC->bShowMouseCursor = false;
				if (PC) PC->SetInputMode(FInputModeGameOnly());
			}
			else
			{
				if (MapSelectInstance->IsInViewport())
				{
					MapSelectInstance->RemoveFromParent();
				}
				PlayerStateInstance->AddToViewport();
				PC->bShowMouseCursor = true;
				if (PC) PC->SetInputMode(Mode);
			}
		}	
	}
	
	return;
}
