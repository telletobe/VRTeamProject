// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "StageInfoWidget.h"
#include "Components/Image.h"

void UMapSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (IM_Map)
    {
        UTexture2D* Map = LoadObject< UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UMG/MapUI/Texture/Map__1_.Map__1_'"));
        if (Map)
        {
            IM_Map->SetBrushFromTexture(Map);
        }
        else
        {
            UE_LOG(LogTemp,Warning,TEXT("Map Texture invalid"));
        }
    }

    if (Button_KyoungGi)
    {
        Button_KyoungGi->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::HandleRegionClicked);
    }
}

void UMapSelectWidget::HandleRegionClicked()
{

    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {
                
        if (WBP_StageInfoWidget)
        {
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp,Warning,TEXT("Call StageInfo"));
        }

    }




    /* 어떤 버튼인지 이름(FName)으로 구분 */
    //if (const UButton* Sender = Cast<UButton>(GetSender()))
    //{
    //    const FName BtnName = Sender->GetFName();        // ex) "Button_Busan"
    //    /* 필요하면 "Button_" 접두어 제거 → "Busan" */
    //    const FString Clean = BtnName.ToString().RightChop(7);
    //    OnRegionSelected.Broadcast(FName(*Clean));       // 델리게이트 전파
    //}
}
