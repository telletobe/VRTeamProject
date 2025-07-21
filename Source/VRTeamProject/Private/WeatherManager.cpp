// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PlayerStateWidget.h"
#include "PlayerCharacter.h"

// Sets default values
AWeatherManager::AWeatherManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWeatherManager::BeginPlay()
{
    Super::BeginPlay();

    RequestKMAWeather();

    //플레이어 캐릭터 참조 저장
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PlayerRef = Cast<APlayerCharacter>(PC->GetPawn());
    }

    //플레이어 위젯 찾기
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (PC->IsLocalController() && PC->GetHUD())
        {
            TArray<UUserWidget*> FoundWidgets;
            UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UPlayerStateWidget::StaticClass(), false);

            if (FoundWidgets.Num() > 0)
            {
                PlayerWidget = Cast<UPlayerStateWidget>(FoundWidgets[0]);
                UE_LOG(LogTemp, Warning, TEXT("PlayerWidget 찾음 및 할당 완료"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("PlayerWidget을 찾지 못함"));
            }
            
        }
    }



}


void AWeatherManager::RequestKMAWeather()
{
	// 현재 시간 기준 포맷된 기상청 시간 문자열 생성
	FString FormattedTime = FDateTime::Now().ToString(TEXT("%Y%m%d%H00"));

    FString URL = FString::Printf(
        TEXT("https://apihub.kma.go.kr/api/typ01/url/kma_sfctm2.php?tm=%s&stn=0&authKey=Y1xuzi_oTlWcbs4v6M5Vjw"),
        *FormattedTime
    );

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));
    Request->OnProcessRequestComplete().BindUObject(this, &AWeatherManager::OnWeatherResponse);
    Request->ProcessRequest();

    UE_LOG(LogTemp, Log, TEXT("[WeatherManager] 날씨 요청 보냄: %s"), *URL);
}

void AWeatherManager::OnWeatherResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[WeatherManager] 날씨 요청 실패"));
        return;
    }

    FString Content = Response->GetContentAsString();

   /* UE_LOG(LogTemp, Log, TEXT("[WeatherManager] 응답 수신:\n%s"), *Content);*/

    // 응답은 JSON이 아니라 '텍스트 테이블'이므로 직접 파싱 필요
    TArray<FString> Lines;
    Content.ParseIntoArrayLines(Lines);

    for (const FString& Line : Lines)
    {
        if (Line.StartsWith(TEXT("#")) || Line.Len() < 20)
        {
            continue; // 주석 또는 빈 줄 무시
        }

        TArray<FString> Columns;
        Line.TrimStartAndEnd().ParseIntoArrayWS(Columns); // 공백 기준 분리

        if (Columns.Num() >= 45)
        {
            float TA = FCString::Atof(*Columns[11]); // 기온 (TA)
            float RN = FCString::Atof(*Columns[15]); // 강수량 (RN)
            float WS = FCString::Atof(*Columns[4]);  // 풍속 (WS)

            CurrentTemp = TA;
            CurrentRain = RN;
            CurrentWindSpeed = WS;

            UE_LOG(LogTemp, Log, TEXT("[WeatherManager] 기온: %.1f°C, 강수량: %.1fmm, 풍속: %.1fm/s"), TA, RN, WS);

            if (PlayerRef)
            {
                PlayerRef->ApplyWeatherAttackDebuff(CurrentTemp, CurrentRain, CurrentWindSpeed);
            }

            if (PlayerWidget)
            {
                UE_LOG(LogTemp, Warning, TEXT("UpdateWeatherInfo 호출: 온도=%.1f, 강수량=%.1f, 풍속=%.1f"), CurrentTemp, CurrentRain, CurrentWindSpeed);

                PlayerWidget->UpdateWeatherInfo(CurrentTemp, CurrentRain, CurrentWindSpeed);
            }

            

            // 예: 게임 날씨 시스템에 적용 (UDS 연동)
            // if (UltraDynamicSky) { UltraDynamicSky->SetRainAmount(FMath::Clamp(RN / 30.0f, 0.0f, 1.0f)); }

            break; // 첫 데이터만 처리
        }
    }
}

