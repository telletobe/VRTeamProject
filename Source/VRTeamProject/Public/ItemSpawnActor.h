// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnActor.generated.h"

class UBoxComponent;
class AVRProjectGameModeBase;
class AGameItem;

UCLASS()
class VRTEAMPROJECT_API AItemSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnActor();
	void SpawnItem();
	void MoveToEndPoint(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void FindTartgetPoint();

	void ChangeActiveState();
	void SetDropTimer();
	void ResetLocationToStartPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnyWhere)
	TObjectPtr<UBoxComponent> ItemSpawnerCollision;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<UStaticMeshComponent> ItemSpawnerMesh;

	UPROPERTY(EditAnyWhere)
	float SpawnDelay;

	UPROPERTY(EditAnyWhere)
	float DropDelay;

	FTimerHandle SpawnItemHandle;
	FTimerHandle ActorVisibleHandle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> StartPoint;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> EndPoint;

	FVector MoveForce;

	bool bIsActive = false;

	TObjectPtr<AVRProjectGameModeBase> GameMode;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameItem> GameItemClass;

	UPROPERTY()
	TObjectPtr<USoundBase> AirPlaneSound;
};
