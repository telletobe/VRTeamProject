// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "WeatherManager.generated.h"

UENUM(BlueprintType)
enum class EWeatherData : uint8
{
	SUN		UMETA(DisplayName = "SUN"),
	RAIN	UMETA(DisplayName = "RAIN"),
	FOGGY	UMETA(DisplayName = "FOGGY")
};

USTRUCT(BlueprintType)
struct FRegionData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Temperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Precipitation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindSpeed;

	FRegionData() : Temperature(20.0f), Precipitation(0.0f), WindSpeed(0.0f), bIsData(false) {}
	FRegionData(float SetTemperature, float SetPrecipitation, float SetWindSpeed) :
		Temperature(SetTemperature), Precipitation(SetPrecipitation), WindSpeed(SetWindSpeed) {
		bIsData = true;
	}

	bool bIsData = false;

	void PrintData() const;

	
};

UCLASS()
class VRTEAMPROJECT_API AWeatherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManager();
	void RequestKMAWeather(float RegionNum);

	UFUNCTION(BlueprintCallable)
	void ClearRegionData();

	UFUNCTION(BlueprintCallable)
	const FRegionData& GetRegionData() const { return RegionData; }

	UFUNCTION(BlueprintCallable)
	EWeatherData GetWeatherData() const { return WeatherData; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:


	void OnWeatherResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SetWeatherData(FRegionData& Data);
	FString SetURLData(int32 RegionNum) const;
	// Weather Data 
	UPROPERTY(EditAnywhere,Category = "Region")
	FRegionData RegionData;

	UPROPERTY(EditAnywhere)
	EWeatherData WeatherData;
};
