// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "WeatherManager.generated.h"

USTRUCT(BlueprintType)
struct FRegionData {
	GENERATED_BODY()

private :
	UPROPERTY(VisibleAnywhere)
	FString LocationName;

	UPROPERTY(VisibleAnywhere)
	float Temperature;

	UPROPERTY(VisibleAnywhere)
	float Precipitation;

	UPROPERTY(VisibleAnywhere)
	float WindSpeed;

public :
	FRegionData() : LocationName(TEXT("Unkown")), Temperature(20.0f), Precipitation(0.0f), WindSpeed(0.0f) {}
	FRegionData(const FString& SetLocationName, float SetTemperature, float SetPrecipitation, float SetWindSpeed) :
		LocationName(SetLocationName), Temperature(SetTemperature), Precipitation(SetPrecipitation), WindSpeed(SetWindSpeed) {}

	void PrintData();
	
};

UCLASS()
class VRTEAMPROJECT_API AWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void RequestKMAWeather(); 
	void OnWeatherResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FString SetURLData(int32 RegionNum);
	// Weather Data 
	FRegionData Suwon;
};
