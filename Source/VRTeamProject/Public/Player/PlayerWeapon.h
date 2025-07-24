// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

class USphereComponent;
class APlayerBulletActor;

UCLASS()
class VRTEAMPROJECT_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();
	void Fire(float Damage);
	void ChangeFireState();
	void SetFireDelay(float AttackFireDelay);

	float GetFireDelay() const { return FireDelay; }
	float GetDefaultFireDelay() const { return DefaultFireDelay; }
	TObjectPtr<UStaticMeshComponent> GetMesh() const { return WeaponMesh.Get(); }
	const FRotator FireWithSpread(float Pitch, float Yaw, float Roll);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> WeaponCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletal;

	bool bIsFire = true;

	UPROPERTY(EditAnywhere)
	float FireDelay = 0.05f;

	float DefaultFireDelay = 0.05f;
	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> FireSound;
};
