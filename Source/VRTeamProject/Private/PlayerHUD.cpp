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

void APlayerHUD::ToggleMapSelect()
{
	if (!MapSelectInstance)
	{
		MapSelectInstance = CreateWidget<UMapSelectWidget>(GetWorld(), MapSelect);
	}

	if (MapSelectInstance->IsInViewport())
	{
		MapSelectInstance->RemoveFromParent();      // 커서 OFF
	}
	else
	{
		MapSelectInstance->AddToViewport();      // 커서 ON
	}
}

void APlayerHUD::PlayerStateShow()
{
	if (IsValid(PlayerState))
	{
		PlayerStateInstance = CreateWidget<UPlayerStateWidget>(GetWorld(), PlayerState);
		PlayerStateInstance->AddToViewport();
	}
}

void APlayerHUD::BeginPlay()
{	



}
