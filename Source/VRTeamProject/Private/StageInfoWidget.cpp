// Fill out your copyright notice in the Description page of Project Settings.


#include "StageInfoWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include <VRProjectGameModeBase.h>
#include <PlayerCharacter.h>
#include "WeatherManager.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UStageInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();



    if (Btn_GameStart)
    {
        Btn_GameStart->OnClicked.AddUniqueDynamic(this, &UStageInfoWidget::GameStart);
    }
    if (Btn_Back)
    {
        Btn_Back->OnClicked.AddUniqueDynamic(this, &UStageInfoWidget::BackToMenu);
    }
}

void UStageInfoWidget::GameStart()
{
    AVRProjectGameModeBase* GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        if (GameMode->IsClear() == false)
        {
            return;
        }
        else
        {        
            // 스트리밍 방식으로 로드
            FLatentActionInfo LatentInfo;
            LatentInfo.CallbackTarget = this;
            LatentInfo.ExecutionFunction = "OnLevelLoaded";
            LatentInfo.Linkage = 0;
            LatentInfo.UUID = __LINE__;

            UGameplayStatics::LoadStreamLevel(this, FName("M_Basic"), true, false, LatentInfo);
            UE_LOG(LogTemp, Warning, TEXT("LoadStreamLevel"));
            SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UStageInfoWidget::BackToMenu()
{
    AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

    if (WeatherManager)
    {
        WeatherManager->ClearRegionData();
    }
    else
    {
        UE_LOG(LogTemp,Warning,TEXT("WeatherManager Invalid"));
    }
    SetVisibility(ESlateVisibility::Hidden);
}

void UStageInfoWidget::OnLevelLoaded()
{
    AVRProjectGameModeBase* GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->TriggerGameStart();
    }
}

void UStageInfoWidget::Init(const FName& InRegionID, UTexture2D* Thumbnail)
{
    RegionID = InRegionID;
    if (TXT_Title)
        TXT_Title->SetText(FText::FromName(RegionID));
    else
        UE_LOG(LogTemp, Warning, TEXT("TXT_Title is nullptr!"));
    if (IMG_Thumb && Thumbnail)
        IMG_Thumb->SetBrushFromTexture(Thumbnail);
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("IMG_Thumb is nullptr!"));
        UE_LOG(LogTemp, Warning, TEXT("Thumbnail is nullptr!"));
    }


}
