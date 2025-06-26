// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "WeatherApiHelper.generated.h"

// ���� ���� �����͸� ������ ����ü
USTRUCT(BlueprintType)
struct WEATHER_API FWeatherCategoryData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Weather")
    FString Category; // PTY, SKY �� ī�װ� �ڵ�

    UPROPERTY(BlueprintReadWrite, Category = "Weather")
    FString Value;    // �ش� ī�װ��� �� (��: "0", "1", "3", "28.9" ��)

    UPROPERTY(BlueprintReadWrite, Category = "Weather")
    FString FcstTime; // ���� �ð� (UltraSrtFcst ����)
};

// ��� ���� ī�װ� �����͸� ���� �����̳�
USTRUCT(BlueprintType)
struct WEATHER_API FCurrentWeatherResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Weather")
    TArray<FWeatherCategoryData> WeatherDataItems;
};

// ���� ���� API ���� �ݹ� ��������Ʈ
// PTY/SKY ���� ���� �����ϵ��� ���� (�������Ʈ���� ���� ���)
DECLARE_DYNAMIC_DELEGATE_FourParams(FOnWeatherInfoReceivedPTY, int32, PTYValue, bool, bIsSuccess, FString, ErrorMessage, const FCurrentWeatherResult&, RawResult);
DECLARE_DYNAMIC_DELEGATE_FourParams(FOnWeatherInfoReceivedSKY, int32, SKYValue, bool, bIsSuccess, FString, ErrorMessage, const FCurrentWeatherResult&, RawResult);

/**
 * 
 */
UCLASS()
class WEATHER_API UWeatherApiHelper : public UObject
{
	GENERATED_BODY()
public:
    // PTY ȣ�� ���� ���
    FOnWeatherInfoReceivedPTY PTYOnCompleted;
    FLatentActionInfo PTYLatentInfo;

    // SKY ȣ�� ���� ���
    FOnWeatherInfoReceivedSKY SKYOnCompleted;
    FLatentActionInfo SKYLatentInfo;

    FString ApiKey;
    int32 CurrentNx;
    int32 CurrentNy;
    FString ApiType; // "PTY" �Ǵ� "SKY"

    // HTTP ��û �Ϸ� �� ȣ��� �ݹ� �Լ�
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


    // JSON ������ �Ľ� �Լ�
    FCurrentWeatherResult ParseWeatherResponse(const FString& JsonString);
    FString GetCurrentBaseTime();
    FString GetCurrentBaseDate();
	
};
