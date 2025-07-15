// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRProjectGameModeBase.generated.h"

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
	void CleanupAfterGameEnd();

	UFUNCTION(BlueprintCallable)
	bool IsClear() const { return bIsClear; }

	bool IsPlayerAlive() const { return bPlayerAlive; }

	void InitializeGameObjects();
	UFUNCTION()
	void CheckGameClear();

	UFUNCTION()
	void CleanupGameItem();
	void CleanupItemSpanwer();
	void CleanupEnemySpawner();

	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	void OnEnemyDeath();

protected:

	virtual void BeginPlay() override;
private:
	bool bIsClear = false;
	bool bPlayerAlive = true;
	bool bItemSpawnerExists = false;
	bool bEnemySpawnerExists = false;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentKillCnt = 0;
	UPROPERTY(EditAnywhere)
	int32 RequiredKillCnt = 20;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemSpawnActor> BPItemSpawner;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemySpawner> BPEnemySpawner;

	UPROPERTY()
	TObjectPtr<AEnemySpawner> EnemySpanwer;

	UPROPERTY()
	TObjectPtr<AItemSpawnActor> ItemSpanwer;

};
