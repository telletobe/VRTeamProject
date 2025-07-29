// Fill out your copyright notice in the Description page of Project Settings.


#include "System/WeatherManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h" 
#include <Kismet/GameplayStatics.h>
#include "EngineUtils.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/EnemySpawner.h"

// Sets default values
AWeatherManager::AWeatherManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<USoundCue> MainBGMObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EffectSound/MainBGM.MainBGM'"));
    static ConstructorHelpers::FObjectFinder<USoundCue> RainBGMObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EffectSound/Rain_Cue.Rain_Cue'"));
    static ConstructorHelpers::FObjectFinder<USoundCue> FoggyBGMObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EffectSound/Foggy_Cue.Foggy_Cue'"));


    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

    if (MainBGMObject.Succeeded())
    {
        MainBGM = MainBGMObject.Object;
    }

    if (RainBGMObject.Succeeded())
    {
        RainBGM = RainBGMObject.Object;
    }

    if (FoggyBGMObject.Succeeded())
    {
        FoggyBGM = FoggyBGMObject.Object;
    }

}

// Called when the game starts or when spawned
void AWeatherManager::BeginPlay()
{
	Super::BeginPlay();
  //  RequestKMAWeather(119);

    if (MainBGM)
    {
        if (AudioComponent)
        {
            AudioComponent = UGameplayStatics::SpawnSound2D(this, MainBGM);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BGMSound Data invalid"));
    }

}

/*
관측소 지점 번호
서울 서울 108
경기 수원 119
강원도 춘천 101
충북 청주 131
충남 홍성 129
전북 전주 146
전남 165
경북 포항 138
경남 창원 155
제주 184
부산 159
광주 156
대전 133
울산 152
인천 112
*/

void AWeatherManager::RequestKMAWeather(const int32 RegionNum)
{
    FString URL = SetURLData(RegionNum); // 관측소 지점 번호는 디스코드 참고.
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));
    Request->OnProcessRequestComplete().BindUObject(this, &AWeatherManager::OnWeatherResponse);
    Request->ProcessRequest();

    UE_LOG(LogTemp, Log, TEXT("[WeatherManager] 날씨 요청 보냄: %s"), *URL);
}

void AWeatherManager::ClearRegionData()
{
    RegionData = FRegionData();
    SetWeatherData(RegionData);

    USoundBase* CurrentSound = AudioComponent ? AudioComponent->GetSound() : nullptr;

    if (AudioComponent && AudioComponent->IsPlaying())
    {
        if (CurrentSound->GetName() != MainBGM->GetName())
        {
            AudioComponent->Stop();
            AudioComponent = UGameplayStatics::SpawnSound2D(this, MainBGM);
        }        
    }
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

            RegionData = FRegionData(Temperture, Precipitation, WindSpeed);
            RegionData.PrintData();

            SetWeatherData(RegionData);

            break; // 첫 데이터만 처리
        }
    }
}

void AWeatherManager::ApplyWeatherEffectToEnemy(const EWeatherData NewWeather) const
{

    UE_LOG(LogTemp,Warning,TEXT("WeatherManager : Applyweather EweatherData : %d"), (int32)NewWeather);

    TArray<AActor*> FoundActor;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), FoundActor);

    for (AActor* AllActor : FoundActor)
    {
        if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AllActor))
        {
            Enemy->ApplyWeatherEffect(NewWeather);
        }
    }
}

void AWeatherManager::TestSetRandomRegionData()
{

    float RandNum = FMath::RandRange(0, 1);

    if (RandNum <= 0.5)
    {
        RegionData.Precipitation = 10.0f;
    }
    else
    {
        RegionData.WindSpeed = 2.0f;
    }

    SetWeatherData(RegionData);

    RegionData.PrintData();
}

void AWeatherManager::SetWeatherData(const FRegionData& Data)
{
    USoundBase* CurrentSound = AudioComponent ? AudioComponent->GetSound() : nullptr;

    Data.PrintData();

    if (Data.Precipitation >= 0.1f)
    {
        WeatherData = EWeatherData::RAIN;
        OnWeatherChange.Broadcast(WeatherData);
        UE_LOG(LogTemp,Warning,TEXT("WeatherManager EWeatherData : %d"),WeatherData);
        if (RainBGM)
        {
            if (AudioComponent && AudioComponent->IsPlaying())
            {
                if (CurrentSound != RainBGM)
                {
                    AudioComponent->Stop();
                    AudioComponent = UGameplayStatics::SpawnSound2D(this, RainBGM);
                }                
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BGMSound Data invalid"));
        }

        return;
    }

    if (Data.WindSpeed >= 0.1f)
    {
        WeatherData = EWeatherData::FOGGY;
        OnWeatherChange.Broadcast(WeatherData);
        if (FoggyBGM)
        {
            if (AudioComponent && AudioComponent->IsPlaying())
            {
                if (CurrentSound != FoggyBGM)
                {
                    AudioComponent->Stop();
                    AudioComponent = UGameplayStatics::SpawnSound2D(this, FoggyBGM);
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BGMSound Data invalid"));
        }
  
        return;
    }
    
    WeatherData = EWeatherData::SUN;
    OnWeatherChange.Broadcast(WeatherData);
    if (MainBGM)
    {
        if (AudioComponent && AudioComponent->IsPlaying())
        {
            if (CurrentSound != MainBGM)
            {
                AudioComponent->Stop();
                AudioComponent = UGameplayStatics::SpawnSound2D(this, MainBGM);
            }
        }
    }
    return;
}

FString AWeatherManager::SetURLData(const int32 RegionNum) const
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

void FRegionData::PrintData() const
{
    UE_LOG(LogTemp,Warning,TEXT("Temperture :  %.1f, Precipitation :  %.1f, WindSpeed : %.1f"),Temperature,Precipitation,WindSpeed);
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::MakeRandomColor(), FString::Printf(TEXT("Temperture :  %.1f, Precipitation :  %.1f, WindSpeed : %.1f"), Temperature, Precipitation, WindSpeed));
}
