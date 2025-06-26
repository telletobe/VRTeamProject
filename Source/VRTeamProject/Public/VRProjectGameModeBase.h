// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRProjectGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReStart);


class AItemSpawnActor;
class AEnemySpawner;
class UWeatherManager;

UCLASS()
class VRTEAMPROJECT_API AVRProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AVRProjectGameModeBase();
public :
	void TriggerGameClear();
	void TriggerGameStart();
	void TriggerGameReStart();
	void CleanupAfterGameClear();
	bool IsClear() const { return bIsClear; }
	bool IsPlayerAlive() const { return bPlayerAlive; }
	void InitializeGameObjects();

	UFUNCTION()
	void CleanupGameItem();

	UFUNCTION()
	void ChangePlayerAliveState();

	UFUNCTION()
	void NotifyReStart();

	FOnReStart OnRestart;
protected:

	virtual void BeginPlay() override;

private:
	bool bIsClear = false;
	bool bPlayerAlive = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemSpawnActor> BPItemSpawner;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemySpawner> BPEnemySpawner;

	UPROPERTY()
	TObjectPtr<UWeatherManager> WeatherManager;

};
