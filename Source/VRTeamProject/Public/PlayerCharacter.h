// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameItem.h"
#include "GameFramework/Character.h"
#include <InputMappingContext.h>
#include "PlayerWeapon.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

class APlayerController;
class UInputManager;

UCLASS()
class VRTEAMPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void SetHp(float PlayerHp);
	void SetAtk(float PlayerAtk);
	void SetDef(float PlayerDef);
	void SetExp(float PlayerExp);

	float GetHp() const { return Hp; }
	float GetAtk() const { return Atk; }
	float GetDef() const { return Def; }
	float GetExp() const { return Exp; }
	float GetMaxHp() const { return MaxHp; }
	TObjectPtr<APlayerWeapon> GetWeapon() const { return *Weapon; }
	bool IsMouseClickedEnable() const {	return bMouseClickEnable;}

	float GetDefaultAtk() const { return DefaultAtk; }
	float GetDefaultDef () const { return DefaultDef; }

	void ApplyEffectItem(EItemEffectData Data);
	void PlayerReSpawn();
	void PlayerDeSpawn();
	void NotifyPlayerDeath();


	UPROPERTY()
	FOnPlayerDeath OnPlayerDeath;

	//void Move(const FInputActionValue& Value);
	//void Look(const FInputActionValue& Value);
	//void Attack(const FInputActionValue& Value);
	//void ToggleMap(const FInputActionValue& Value);
	//void PlayerStat(const FInputActionValue& Value);
	//void Click(const FInputActionValue& Value);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
	float Hp;

	UPROPERTY(VisibleAnywhere)
	float MaxHp = 100.0f;

	UPROPERTY(EditAnywhere)
	float Atk;

	float DefaultAtk = 5.0f;

	UPROPERTY(EditAnywhere)
	float Def;

	float DefaultDef = 1.0f;

	UPROPERTY(VisibleAnywhere)
	float Exp;

	//bool bIsArrived;
	bool bIsActive = true;
	bool bMouseClickEnable = true;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<APlayerWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<UInputManager> InputManager;

};
