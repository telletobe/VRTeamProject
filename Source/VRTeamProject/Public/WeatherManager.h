// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Http.h"
#include "WeatherManager.generated.h"

class UPlayerStateWidget;
class APlayerCharacter;

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

	// Weather Data 
	float CurrentTemp;
	float CurrentRain;
	float CurrentWindSpeed;

	UPROPERTY()
	UPlayerStateWidget* PlayerWidget;

	UPROPERTY()
	APlayerCharacter* PlayerRef;

	

public:
	void SetPlayerWidget(UPlayerStateWidget* Widget)
	{
		PlayerWidget = Widget;
	}

};
