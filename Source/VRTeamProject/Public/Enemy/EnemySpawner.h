// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawned, AEnemyCharacter*, Enemy);

class UBoxComponent;
class AEnemyCharacter;
class AVRProjectGameModeBase;
class ATargetPoint;

UCLASS()
class VRTEAMPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	void CreateEnemy();
	void DestroyEnemy();
	TArray<AEnemyCharacter*>& GetEnemyPool() { return EnemyPool; }
	float GetSpawnDelay() const { return SpawnDelay; }

	void DeActivateEnemySpawner();

	void ActivateEnemySpawner();

	UFUNCTION()
	void SpawnEnemy();

	UPROPERTY()
	FOnEnemySpawned OnEnemySpawned;

	UFUNCTION()
	void OnWeatherChanged(EWeatherData NewWeather);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private :

	const static int32 EnemyPoolSize;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> SpawnBox;

	FTimerHandle SpawnHandle;

	TArray<AEnemyCharacter*> EnemyPool;

	UPROPERTY(EditAnywhere, category = "EnemyPool")
	float SpawnDelay = 0.7f;

	int32 PoolIndex = 0;

	bool bIsClear = false;

	UPROPERTY()
	EWeatherData CachedWeatherData;
};
