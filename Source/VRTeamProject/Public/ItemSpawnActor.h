// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnActor.generated.h"

class UBoxComponent;


UCLASS()
class VRTEAMPROJECT_API AItemSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnActor();
	void SpawnItem();

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
	float SpawnDelay = 3.0f;

	FTimerHandle SpawnHandle;

};
