// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyKilled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeathAnimEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyAttack,float, Atk);

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
	void SetSpawnPoint(AActor* TargetPoint);

	float GetCurrentHp() const { return CurrentHp; }
	float GetMaxHp() const { return MaxHp; }
	float GetDef() const { return Def; }
	float GetAtk() const { return Atk; }
	float GetSpawnDelay() const { return SpawnDelay; }

	bool IsActive() const { return bIsActive; }
	bool IsDeathAnim() const { return bIsDeathAnim; }

	UFUNCTION(BlueprintCallable, Category = "Anim")
	bool IsAttacking() const { return bIsAttacking; }

	UFUNCTION(BlueprintCallable, Category = "Anim")
	bool IsHitReacting() const { return bIsHitReacting; }

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DeSpawn();
	void Spawn();

	void BroadcastEnemyKilled();
	void BoradCastEnemyAttack();

	UPROPERTY()
	FOnEnemyAttack OnEnemyAttack;

	UPROPERTY()
	FOnEnemyKilled OnEnemyKilled;

	UPROPERTY()
	FOnEnemyDeathAnimEnded OnEnemyDeathAnimEnded;

	UFUNCTION()
	void PlayDeathMontage();

	UFUNCTION()
	void PlayHitMontage();

	UFUNCTION()
	void PlayAttackMontage();

	UFUNCTION()
	void EnemyDeathAnimEnded();

	UFUNCTION(BlueprintCallable)
	void FindSpawnPoint();
	void FindDeSpawnPoint();

	UFUNCTION()
	void PlayHitEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


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

	float NavGenerationRadius;
	float NavRemovalRadius;

	UPROPERTY(EditAnywhere)
	float CurrentHp;

	UPROPERTY(EditAnywhere)
	float MaxHp = 50.0f;

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


	UPROPERTY(EditAnywhere, Category = "Anim")
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, Category = "Anim")
	bool bIsHitReacting = false;

	UPROPERTY(EditAnywhere, Category = "Anim")
	bool bIsMoving = false;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;


	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HitMontage;

	FTimerHandle HitAnimTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	FTimerHandle AttackAnimTimerHandle;

};
