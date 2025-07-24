// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MapSelectWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Widget/StageInfoWidget.h"
#include "Components/Image.h"
#include "System/WeatherManager.h"
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

void UMapSelectWidget::RequestWeatherData(const int32 RegionNum)
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {

        AWeatherManager* WeatherManager = Cast<AWeatherManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWeatherManager::StaticClass()));

        if (WeatherManager)
        {
            WeatherManager->RequestKMAWeather(RegionNum); //���ڿ��´� ���� ������û
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WeatherManager Invalid"));
        }
    }
}

void UMapSelectWidget::RequestRegionImage(const FName& RegionName)
{
    if (APlayerController* PC = Cast<APlayerController>(GetOwningPlayer()))
    {
        const FName& Region = RegionName;
        UTexture2D* Thumbnail = RegionThumbnails.Contains(Region) ? RegionThumbnails[Region] : nullptr;
        WBP_StageInfoWidget->Init(Region, Thumbnail);
        WBP_StageInfoWidget->SetVisibility(ESlateVisibility::Visible);

        UE_LOG(LogTemp, Warning, TEXT("Call StageInfo"));
    }
}


void UMapSelectWidget::OnClickKyounGi()
{
    RequestWeatherData(119); //��� ����
    RequestRegionImage("KyougGi");
}

void UMapSelectWidget::OnClickJeonju()
{
    RequestWeatherData(146); //����
    RequestRegionImage("JeonJu");

}

void UMapSelectWidget::OnClickKangwon()
{
    RequestWeatherData(101); //������
    RequestRegionImage("Gangwon");

}

void UMapSelectWidget::OnClickcheongju()
{
    RequestWeatherData(131); //û��
    RequestRegionImage("ChungBuk");

}

void UMapSelectWidget::OnClickHongseong()
{
    RequestWeatherData(129); //ȫ��
    RequestRegionImage("ChungNam");

}

void UMapSelectWidget::OnClickdaejeon()
{
    RequestWeatherData(133); //����
    RequestRegionImage("Daejeon");

}

void UMapSelectWidget::OnClickPohang()
{
    RequestWeatherData(138); // ����
    RequestRegionImage("KyoungBuk");

}

void UMapSelectWidget::OnClickChangwon()
{
    RequestWeatherData(155); //â��
    RequestRegionImage("KyoungNam");

}

void UMapSelectWidget::OnClickBusan()
{
    RequestWeatherData(159); //�λ�
    RequestRegionImage("Busan");

}

void UMapSelectWidget::OnClickSeoul()
{
    RequestWeatherData(108); //����
    RequestRegionImage("Seoul");
}
