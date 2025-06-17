// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"


void UMapSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (Button_KyoungGi)
    {
        Button_KyoungGi->OnClicked.AddDynamic(this, &UMapSelectWidget::HandleRegionClicked);
    }

}

void UMapSelectWidget::HandleRegionClicked()
{
    /* � ��ư���� �̸�(FName)���� ���� */
    //if (const UButton* Sender = Cast<UButton>(GetSender()))
    //{
    //    const FName BtnName = Sender->GetFName();        // ex) "Button_Busan"
    //    /* �ʿ��ϸ� "Button_" ���ξ� ���� �� "Busan" */
    //    const FString Clean = BtnName.ToString().RightChop(7);
    //    OnRegionSelected.Broadcast(FName(*Clean));       // ��������Ʈ ����
    //}

    
}
