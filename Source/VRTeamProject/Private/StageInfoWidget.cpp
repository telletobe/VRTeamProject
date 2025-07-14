// Fill out your copyright notice in the Description page of Project Settings.


#include "StageInfoWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include <VRProjectGameModeBase.h>
#include <PlayerCharacter.h>
#include "WeatherManager.h"

void UStageInfoWidget::Init(const FName& InRegionID, UTexture2D* Thumbnail, int32 Difficulty)
{
   /* RegionID = InRegionID;
    if (TXT_Title)      
        TXT_Title->SetText(FText::FromName(RegionID));
    if (IMG_Thumb)      
        IMG_Thumb->SetBrushFromTexture(Thumb);
    if (TXT_Difficulty) 
        TXT_Difficulty->SetText(FText::AsNumber(Diff));*/
}

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
            // ��Ʈ���� ������� �ε�
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
