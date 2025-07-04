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
	
	UFUNCTION(BlueprintCallable)
	void TriggerGameStart();
	void TriggerGameReStart();
	void CleanupAfterGameClear();
	bool IsClear() const { return bIsClear; }
	bool IsPlayerAlive() const { return bPlayerAlive; }


	void InitializeGameObjects();
	UFUNCTION()
	void CheckGameClear();


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
	bool bItemSpawnerExists = false;
	bool bEnemySpawnerExists = false;

	int32 CurrentKillCnt = 0;
	int32 RequiredKillCnt = 20;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemSpawnActor> BPItemSpawner;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemySpawner> BPEnemySpawner;

	UPROPERTY()
	TObjectPtr<AEnemySpawner> Spanwer;

	UPROPERTY()
	TObjectPtr<UWeatherManager> WeatherManager;

};
