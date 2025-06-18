// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
//TEST
#include "VRProjectGameModeBase.h"
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
    /* 어떤 버튼인지 이름(FName)으로 구분 */
    //if (const UButton* Sender = Cast<UButton>(GetSender()))
    //{
    //    const FName BtnName = Sender->GetFName();        // ex) "Button_Busan"
    //    /* 필요하면 "Button_" 접두어 제거 → "Busan" */
    //    const FString Clean = BtnName.ToString().RightChop(7);
    //    OnRegionSelected.Broadcast(FName(*Clean));       // 델리게이트 전파
    //}
    AVRProjectGameModeBase* GM = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
    GM->TriggerGameClear();
    GM->TriggerGameStart();
    
}
