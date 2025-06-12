// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"
#include "PlayerCharacter.h"


void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_Quit)
	{
		btn_Quit->OnClicked.AddDynamic(this, &UPlayerStateWidget::GameQuit);
	}
    if (btn_Option)
    {
        btn_Option->OnClicked.AddDynamic(this, &UPlayerStateWidget::Option);
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


