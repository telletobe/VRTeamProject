// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "MapselectWidget.h"
#include "PlayerStateWidget.h"
#include "StageInfoWidget.h"
#include "Blueprint/UserWidget.h"
#include <EndGameWidget.h>

/*
	UserWidgetclass�� ����
*/

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

	static ConstructorHelpers::FClassFinder<UStageInfoWidget> StageInfoWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/MapUI/WBP_StageInfoWidget.WBP_StageInfoWidget_C'"));

	if (StageInfoWidget.Succeeded())
	{
		StageInfo = StageInfoWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UEndGameWidget> EndGameWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/EndGameWidget.EndGameWidget_C'"));

	if (EndGameWidget.Succeeded())
	{
		EndGame = EndGameWidget.Class;
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
		MapSelectInstance->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayerState && !PlayerStateInstance)
	{
		PlayerStateInstance = CreateWidget<UPlayerStateWidget>(GetWorld(), PlayerState);
		PlayerStateInstance->SetVisibility(ESlateVisibility::Visible);
	}

	if (StageInfo && !StageInfoInstance)
	{
		StageInfoInstance = CreateWidget<UStageInfoWidget>(GetWorld(), StageInfo);
	}

	if (EndGame && !EndGameInstance)
	{
		EndGameInstance = CreateWidget<UEndGameWidget>(GetWorld(), EndGame);
	}

	Mode.SetHideCursorDuringCapture(false);
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	EndGameInstance->AddToViewport();
	EndGameInstance->RemoveFromParent();

}

TObjectPtr<UMapSelectWidget> APlayerHUD::GetMapSelectInstance() const
{
	if(MapSelectInstance) return MapSelectInstance.Get();

	return nullptr;
}

TObjectPtr<UPlayerStateWidget> APlayerHUD::GetPlayerStateInstance() const
{
	if (PlayerStateInstance) return PlayerStateInstance.Get();
	
	return nullptr;
}

TObjectPtr<UStageInfoWidget> APlayerHUD::GetStageInfoInstance() const
{
	if (StageInfoInstance) return StageInfoInstance.Get();
	return nullptr;
}

TObjectPtr<UEndGameWidget> APlayerHUD::GetEndGameInstance() const
{
	if (EndGameInstance) return EndGameInstance.Get();
	return nullptr;
}
 