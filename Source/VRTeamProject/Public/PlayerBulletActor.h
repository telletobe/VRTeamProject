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
	void SetDamage(float BulletDamage);
	float GetDamage() const { return Damage; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex,	bool bFromSweep,const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private :

	UPROPERTY()
	TObjectPtr<USphereComponent> BulletCollision;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BulletMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> BulletMeshAsset;

	UPROPERTY()
	TObjectPtr<UMaterial> BulletMeshMaterial;

	float BulletSpeed = 1500.0f;
	float MoveInterval = 0.01f;
	float Damage = 0.0f;
};
