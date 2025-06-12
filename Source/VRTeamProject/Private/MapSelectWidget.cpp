// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"



void UMapSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (SelectButton_1)
    {
        SelectButton_1->OnClicked.AddDynamic(this, &UMapSelectWidget::EntertoStage);
    }
}

void UMapSelectWidget::EntertoStage()
{

}