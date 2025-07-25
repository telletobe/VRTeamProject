// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulidingBaseActor.generated.h"

class UBoxComponent;
class ATargetPoint;

UCLASS()
class VRTEAMPROJECT_API ABulidingBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulidingBaseActor();

	void SetIsArrive(bool IsArrive);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void MovetoTarget(float DeltaTime);
	virtual void BackToStartPoint(FVector Location);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BulidingCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BulidRight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BulidLeft;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BulidMiddle;

	UPROPERTY(EditAnywhere, category = "EndPoint")
	TObjectPtr<ATargetPoint> EndPoint;

	bool bIsArrive = false;
	FVector MoveForce;
	FVector StartLocation;


};
