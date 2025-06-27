// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

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
	void SpawnEnemy();
	TArray<AEnemyCharacter*>& GetEnemyPool() { return EnemyPool; }
	FTimerHandle& GetSpawnHandle() { return SpawnHandle; }
	float GetSpawnDelay() const { return SpawnDelay; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

	void DeActivateEnemySpawner();

	UFUNCTION()
	void ActivateEnemySpawner();
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
};
