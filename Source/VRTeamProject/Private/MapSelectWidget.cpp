// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"

void UMapSelectWidget::OnMyButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("버튼이 눌렸습니다!"));
}

void UMapSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectButton_1)
	{
		SelectButton_1->OnClicked.AddDynamic(this,&UMapSelectWidget::OnMyButtonClicked);
	}
}
