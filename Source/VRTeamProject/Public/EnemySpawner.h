// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;
class AEnemyCharacter;
class AVRProjectGameModeBase;

UCLASS()
class VRTEAMPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	void CreateEnemy();

	UFUNCTION()
	void CheckGameClear(AEnemyCharacter* Enemy);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private :

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> SpawnBox;

	FTimerHandle CreateHandle;
	FTimerHandle SpawnHandle;

	TArray<AEnemyCharacter*> EnemyPool;

	UPROPERTY(EditAnywhere, category = "EnemyPool")
	float CreateDelay = 3.0f;

	static int32 EnemyPoolSize;

	int32 CurrentKillCnt = 0;
	int32 RequiredKillCnt = 10;
	bool bIsClear = false;

	TObjectPtr<AVRProjectGameModeBase> GameMode;

};
