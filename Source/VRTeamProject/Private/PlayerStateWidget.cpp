// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"


void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_Quit)
	{

		btn_Quit->OnClicked.AddUniqueDynamic(this, &UPlayerStateWidget::GameQuit);
		
		
	}
    if (btn_Option)
    {
		btn_Option->OnClicked.AddUniqueDynamic(this, &UPlayerStateWidget::Option);

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


