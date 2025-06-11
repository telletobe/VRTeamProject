// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"


void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_Pause)
	{
		btn_Pause->OnClicked.AddDynamic(this, &UPlayerStateWidget::GamePause);
	}


}

void UPlayerStateWidget::GamePause()
{

}

void UPlayerStateWidget::UpdatePlayerStats()
{
}


