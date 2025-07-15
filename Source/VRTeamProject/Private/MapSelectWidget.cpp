// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "StageInfoWidget.h"
#include "Components/Image.h"
#include "WeatherManager.h"
#include "Kismet/GameplayStatics.h"

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
        Button_KyoungGi->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickKyounGi);
    }

    if (Button_Jeonju)
    {
        Button_Jeonju->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickJeonju);
    }

    if (Button_Kangwon)
    {
        Button_Kangwon->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickKangwon);
    }
}

void UMapSelectWidget::OnClickKyounGi()
{

    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(119); //수원 정보요청
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }

}

void UMapSelectWidget::OnClickJeonju()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(146); //전라도 전주 정보요청
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickKangwon()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(101); //강원도 정보요청
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}
