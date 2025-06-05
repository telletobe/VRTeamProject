// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "MapselectWidget.h"
#include "PlayerStateWidget.h"
#include "Blueprint/UserWidget.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UMapSelectWidget> Selectwidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/WBP_MapSelectWidget.WBP_MapSelectWidget_C'"));

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
	//
	//if (isvalid(mapselect))
	//{
	//	mapselectinstance = createwidget<umapselectwidget>(getworld(), mapselect);
	//	mapselectinstance->addtoviewport();
	//}

	//if (IsValid(PlayerState))
	//{
	//	PlayerStateInstance = CreateWidget<UPlayerStateWidget>(GetWorld(), PlayerState);
	//	PlayerStateInstance->AddToViewport();
	//}



}
