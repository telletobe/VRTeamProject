// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherManager.h"
#include "WeatherApiHelper.h"
#include "WeatherBPLibrary.h"

TObjectPtr<UWeatherManager> UWeatherManager::WeatherInstance = nullptr;

UWeatherManager::UWeatherManager()
{

}

UWeatherManager* UWeatherManager::GetInstance()
{
	if (!WeatherInstance)
	{
		WeatherInstance = NewObject<UWeatherManager>();
		WeatherInstance->AddToRoot();
	}
	return WeatherInstance.Get();
}

void UWeatherManager::Init()
{
	FString apiKey = TEXT("PKLCjvKc2RgztD1XwdYUS7cHkacT3rE5cn1CvpTpFGxKyoIw79%2BMRd1M1O74w2fokbapdV7Maym5Kd%2BRS00vKw%3D%3D");
	FLatentActionInfo LatenInfo;
	FOnWeatherInfoReceivedPTY ReceivedPTY;
	FOnWeatherInfoReceivedSKY ReceivedSKY;

	ReceivedPTY.BindDynamic(this, &UWeatherManager::ReceivedPTYValue);
	ReceivedSKY.BindDynamic(this, &UWeatherManager::ReceivedSKYValue);

	//Get Data
	if (CurrentWorld)
	{
		//숫자는 경도,위도 액셀 참조
		UWeatherBPLibrary::GetPTY(CurrentWorld, apiKey, 60, 127, ReceivedPTY, LatenInfo);
		UWeatherBPLibrary::GetSKY(CurrentWorld, apiKey, 60, 127, ReceivedSKY, LatenInfo);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("World context is not set. Please call SetWorld() before using this Function."));
		return;
	}
}

void UWeatherManager::SetCurrentWorld(UWorld* CurWorld)
{
	CurrentWorld = CurWorld;
}

void UWeatherManager::ReceivedPTYValue(int32 PTYValue, bool bIsSuccess, FString ErrorMessage, const FCurrentWeatherResult& RawResult)
{
	if (bIsSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Received PTY value = %d"), PTYValue);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to receive PTY value = %s"), *ErrorMessage);
	}
}

void UWeatherManager::ReceivedSKYValue(int32 SKYValue, bool bIsSuccess, FString ErrorMessage, const FCurrentWeatherResult& RawResult)
{
	if (bIsSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Received SKY value = %d"), SKYValue);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to receive SKY value = %s"), *ErrorMessage);
	}
}

