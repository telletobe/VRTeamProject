// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"

void UMapSelectWidget::OnMyButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("��ư�� ���Ƚ��ϴ�!"));
}

void UMapSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectButton_1)
	{
		SelectButton_1->OnClicked.AddDynamic(this,&UMapSelectWidget::OnMyButtonClicked);
	}
}
