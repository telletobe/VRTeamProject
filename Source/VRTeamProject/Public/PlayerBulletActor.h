// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBulletActor.generated.h"

class USphereComponent;

UCLASS()
class VRTEAMPROJECT_API APlayerBulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBulletActor();
	void BulletMove();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private :

	UPROPERTY()
	TObjectPtr<USphereComponent> BulletCollision;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BulletMesh;

	float BulletSpeed = 1500.0f;
	float MoveInterval = 0.01f;
};
