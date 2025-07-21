// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "VRProjectGameModeBase.h"





void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		if (Player)
		{
			Player->OnHealthChange.AddUniqueDynamic(this,&UPlayerStateWidget::UpdatePlayerHP);
		}
	}	
}

float UPlayerStateWidget::CalculateAttackPowerPercent(float Temperature, float Rainfall, float WindSpeed) const
{
	TArray<float> DebuffValues;

	if (Temperature >= 30.0f)
	{
		DebuffValues.Add(50.0f);
	}
	if (Rainfall >= 100.0f)
	{
		DebuffValues.Add(30.0f); // 남은 공격력 70% -> 100 * 0.7 = 70
	}
	if (WindSpeed >= 10.0f)
	{
		DebuffValues.Add(10.0f); // 남은 공격력 90% -> 100 * 0.9 = 90
	}

	if (DebuffValues.Num() == 0)
	{
		return 100.0f;
	}
	else
	{
		return FMath::Min(DebuffValues);
	}
}

void UPlayerStateWidget::ReSetHp() //Restart 버튼을 눌럿을떄 활용
{
	if (HPBar)
	{
		HPBar->SetPercent(1.0f);
	}
}


void UPlayerStateWidget::UpdatePlayerHP(float CurrentHp, float MaxHp)
{
	if (HPBar && MaxHp > 0.0f)
	{
		HPBar->SetPercent(CurrentHp/MaxHp);
	}
	//Set Text
}

void UPlayerStateWidget::UpdatePlayerDef()
{
	if (DefBar)
	{
		DefBar->SetPercent(1.0f);
	}
	//Set Text
}

void UPlayerStateWidget::UpdatePlayerExp(float CurrentExp)
{
	if (ExpBar)
	{
		ExpBar->SetPercent(0.0f);
	}
	//Set Text
}

void UPlayerStateWidget::UpdateWeatherInfo(float InTemperature, float InRainfall, float InWindSpeed)
{
	if (TemperatureText)
	{
		TemperatureText->SetText(FText::FromString(FString::Printf(TEXT(" %.1f°C"), InTemperature)));
	}

	if (RainfallText)
	{
		RainfallText->SetText(FText::FromString(FString::Printf(TEXT(" %.1fmm"), InRainfall)));
	}

	if (WindSpeedText)
	{
		WindSpeedText->SetText(FText::FromString(FString::Printf(TEXT(" %.1fm/s"), InWindSpeed)));
	}

	float AttackPercent = CalculateAttackPowerPercent(InTemperature, InRainfall, InWindSpeed);

	if (NowPlayerAttackText)
	{
		NowPlayerAttackText->SetText(FText::FromString(FString::Printf(TEXT(" %.0f%%"), AttackPercent)));
	}

}
