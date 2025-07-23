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

    if (Button_cheongju)
    {
        Button_cheongju->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickcheongju);
    }

    if (Button_Hongseong)
    {
        Button_Hongseong->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickHongseong);
    }
     
    if (Button_daejeon)
    {
        Button_daejeon->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickdaejeon);;
    }

    if (Button_Pohang)
    {
        Button_Pohang->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickPohang);
    }

    if (Button_Changwon)
    {
        Button_Changwon->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickChangwon);
    }

    if (Button_Busan)
    {
        Button_Busan->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickBusan);
    }

    if (Button_Seoul) // Special Stage
    {
        Button_Seoul->OnClicked.AddUniqueDynamic(this, &UMapSelectWidget::OnClickSeoul);
    }
}

void UMapSelectWidget::OnClickKyounGi()
{

    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(119); //���� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "KyougGi";
            UTexture2D* Thumbnail = nullptr;
            if (RegionThumbnails.Contains(Region))
            {
                Thumbnail = RegionThumbnails[Region];
            }

            WBP_StageInfoWidget->Init(Region, Thumbnail);
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
            WeatherManager->RequestKMAWeather(146); //���� ���� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "JeonJu";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
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
            WeatherManager->RequestKMAWeather(101); //������ ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "Gangwon";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickcheongju()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(131); //��� û�� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "ChungBuk";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickHongseong()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(129); //�泲 ȫ�� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "ChungNam";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickdaejeon()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(133); //������ ���� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "Daejeon";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickPohang()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(138); //���ϵ� ���� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "KyoungBuk";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickChangwon()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(155); //��󳲵� â�� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "KyoungNam";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickBusan()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(159); //�λ� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "Busan";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}

void UMapSelectWidget::OnClickSeoul()
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(108); //���� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }

        if (WBP_StageInfoWidget)
        {
            FName Region = "Seoul";
            UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
            WBP_StageInfoWidget->Init(Region, Thumbnail);
            WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

            UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
        }

    }
}
