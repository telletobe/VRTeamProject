// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"

void UPlayerStateWidget::UpdateHP()
{
	UE_LOG(LogTemp, Warning, TEXT("��ư�� ���Ƚ��ϴ�!"));
}

void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_0)
	{
		Button_0->OnClicked.AddDynamic(this, &UPlayerStateWidget::UpdateHP);
	}
}


