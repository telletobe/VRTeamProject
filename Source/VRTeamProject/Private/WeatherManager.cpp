// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"


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
}


void AWeatherManager::RequestKMAWeather()
{
    FString URL = SetURLData(119); // 관측소 지점 번호는 디스코드 참고.
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

    // ✅ 응답은 JSON이 아니라 '텍스트 테이블'이므로 직접 파싱 필요
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
            float Temperture = FCString::Atof(*Columns[11]); // 기온 (TA)
            float Precipitation = FCString::Atof(*Columns[15]); // 강수량 (RN)
            float WindSpeed = FCString::Atof(*Columns[4]);  // 풍속 (WS)

            Suwon = FRegionData("Suwon", Temperture, Precipitation, WindSpeed);
            Suwon.PrintData();

            // 예: 게임 날씨 시스템에 적용 (UDS 연동)
            // if (UltraDynamicSky) { UltraDynamicSky->SetRainAmount(FMath::Clamp(RN / 30.0f, 0.0f, 1.0f)); }

            break; // 첫 데이터만 처리
        }
    }
}

FString AWeatherManager::SetURLData(int32 RegionNum)
{
    // 현재 시간 기준 포맷된 기상청 시간 문자열 생성
    FString FormattedTime = FDateTime::Now().ToString(TEXT("%Y%m%d%H00"));

    FString URL = TEXT("");
    FString URLFront = FString::Printf(TEXT("https://apihub.kma.go.kr/api/typ01/url/kma_sfctm2.php?tm=%s&stn="), *FormattedTime);
    FString URLBack = FString::Printf(TEXT("&authKey=Y1xuzi_oTlWcbs4v6M5Vjw"));

    URL.Append(URLFront);
    URL.Append(FString::FromInt(RegionNum));
    URL.Append(URLBack);

    return URL;
}

void FRegionData::PrintData()
{
    UE_LOG(LogTemp,Warning,TEXT("region : %s,Temperture :  %.1f, Precipitation :  %.1f, WindSpeed : %.1f"), *LocationName,Temperature,Precipitation,WindSpeed);
}
