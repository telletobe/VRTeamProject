// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDespawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);

class AAIController;
class UNavigationInvokerComponent;
class ATargetPoint;

UCLASS()
class VRTEAMPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// NavInvoker Subobject return forcely
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return NavInvoker; }

	void SetCurrentHp(float EnemyHp);
	void SetMaxHp(float EnemyHp);
	void SetDef(float EnemyDef);
	void SetAtk(float EnemyAtk);
	void SetSpawnDelay(float EnemySpawnDelay);

	float GetCurrentHp() const { return CurrentHp; }
	float GetMaxHp() const { return MaxHp; }
	float GetDef() const { return Def; }
	float GetAtk() const { return Atk; }
	float GetSpawnDelay() const { return SpawnDelay; }

	bool IsActive() const { return bIsActive; }
	bool IsDeathAnim() const { return bIsDeathAnim; }

	void DeSpawn();
	void Spawn();

	void NotifyEnemyDespawn();
	void NotifyEnemyDeath();
	
	UPROPERTY()
	FOnEnemyDespawned OnEnemyDespawned;

	UPROPERTY()
	FOnEnemyDeath OnEnemyDeath;


	UFUNCTION()
	void PlayDeathMontage();

	UFUNCTION()
	void DeathMontageEnded();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private :
	void FindSpawnPoint();
	void FindDeSpawnPoint();



public :

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	//Invoker setting
	UPROPERTY(EditAnywhere, Category = Navigation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNavigationInvokerComponent> NavInvoker;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> DeSpawnPoint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> SpawnPoint;

	float NavGenerationRadius; // 동적으로 초록색 칸을 생성
	float NavRemovalRadius; // 동적으로 초록색 칸을 제거

	UPROPERTY(EditAnywhere)
	float CurrentHp;

	UPROPERTY(EditAnywhere)
	float MaxHp = 10.0f;

	UPROPERTY(EditAnywhere)
	float Def;

	float DefaultDef = 1.0f;

	UPROPERTY(EditAnywhere)
	float Atk;

	float DefaultAtk = 3.0f;
	
	UPROPERTY(EditAnywhere)
	float SpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Anim")
	bool bIsActive = false;

	UPROPERTY(EditAnywhere, Category = "Anim")
	bool bIsDeathAnim = false;


	//Death 애님관련
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;

	FTimerHandle DeathAnimTimerHandle;

};
