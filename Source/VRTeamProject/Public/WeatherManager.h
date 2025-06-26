// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeatherManager.generated.h"

struct FCurrentWeatherResult;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UWeatherManager : public UObject
{
	GENERATED_BODY()
	
public:
	static UWeatherManager* GetInstance();
	UWorld& GetCurrentWorld() const { return *CurrentWorld; }
	void SetCurrentWorld(UWorld* CurWorld);

	UFUNCTION()
	void ReceivedPTYValue(int32 PTYValue, bool bIsSuccess, FString ErrorMessage, const FCurrentWeatherResult& RawResult);

	UFUNCTION()
	void ReceivedSKYValue(int32 SKYValue, bool bIsSuccess, FString ErrorMessage, const FCurrentWeatherResult& RawResult);

	void Init();

protected:

private:
	UWeatherManager();

private:
	static TObjectPtr<UWeatherManager> WeatherInstance;
	TObjectPtr<UWorld> CurrentWorld;
};
