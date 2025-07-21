// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateWidget.generated.h"

class UProgressBar;
class UButton;
class UTextBlock;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void UpdatePlayerHP(float CurrentHp,float MaxHp);

	UFUNCTION()
	void ReSetHp();

	void UpdatePlayerDef();
	void UpdatePlayerExp(float CurrentExp);

	// 날씨 정보 UI 업데이트
	void UpdateWeatherInfo(float InTemperature, float InRainfall, float InWindSpeed);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> DefBar;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> ExpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TemperatureText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RainfallText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WindSpeedText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NowPlayerAttackText;

	// 내부 계산 함수
	float CalculateAttackPowerPercent(float Temperature, float Rainfall, float WindSpeed) const;



};