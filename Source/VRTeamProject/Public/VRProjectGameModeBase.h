// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRProjectGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDied);


class AItemSpawnActor;
class AEnemySpawner;

UCLASS()
class VRTEAMPROJECT_API AVRProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AVRProjectGameModeBase();
public :
	UFUNCTION(BlueprintCallable)
	void TriggerGameClear();
	
	UFUNCTION(BlueprintCallable)
	void TriggerGameStart();

	UFUNCTION(BlueprintCallable)
	void TriggerGameReStart();

	UFUNCTION(BlueprintCallable)
	void CleanupAfterGameEnd();

	bool IsClear() const { return bIsClear; }

	bool IsPlayerAlive() const { return bPlayerAlive; }

	void InitializeGameObjects();
	UFUNCTION()
	void CheckGameClear();

	UFUNCTION()
	void OnEnemySpawned(class AEnemyCharacter* SpawnedEnemy);
	UFUNCTION()
	void CleanupGameItem();

	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	void NotifyReStart();

	UFUNCTION()
	void DeActivateEnemySpawner();

	UPROPERTY()
	FOnReStart OnRestart;

	UPROPERTY()
	FOnPlayerDied OnPlayerDied;
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

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* MainBGM;


};
