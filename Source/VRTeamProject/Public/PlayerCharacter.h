// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameItem.h"
#include "GameFramework/Character.h"
#include <InputMappingContext.h>
#include "PlayerWeapon.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange,float,CurrentHp,float,MaxHp);

class APlayerController;
class UInputManager;
class UMotionControllerComponent;
class UWidgetInteractionComponent;
class UCameraComponent;
class UWidgetComponent;
class UMapSelectWidget;
class USpringArmComponent;

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
	void HideWidgetComponent();

	float GetHp() const { return Hp; }
	float GetAtk() const { return Atk; }
	float GetDef() const { return Def; }
	float GetExp() const { return Exp; }
	float GetMaxHp() const { return MaxHp; }

	bool IsActive() const { return bIsActive; }

	TObjectPtr<APlayerWeapon> GetWeapon() const { return *Weapon; }
	bool IsMouseClickedEnable() const {	return bMouseClickEnable;}

	float GetDefaultAtk() const { return DefaultAtk; }
	float GetDefaultDef () const { return DefaultDef; }

	void ApplyEffectItem(const EItemEffectData& Data);

	UFUNCTION()
	void PlayerReSpawn();

	UFUNCTION()
	void TakenDamage(float Damage);

	void PlayerDeSpawn();
	void NotifyPlayerDeath();

	void NotifyPlayerChangeHealth();


	UPROPERTY()
	FOnHealthChange OnHealthChange;

	// VR
	//fdfd

	TObjectPtr<UStaticMeshComponent> GetMotionControllerLeftLazerMesh() const { return MotionControllerLeftLazerMesh.Get(); }
	TObjectPtr<UStaticMeshComponent> GetMotionControllerRightLazerMesh() const { return MotionControllerRightLazerMesh.Get(); }
	TObjectPtr<UMotionControllerComponent> GetMotionControllerLeft() const { return *MotionControllerLeft; }
	TObjectPtr<UMotionControllerComponent> GetMotionControllerRight() const { return *MotionControllerRight; }
	TObjectPtr<UWidgetInteractionComponent> GetWidgetInteractionLeft() const { return *WidgetInteractionLeft; }
	TObjectPtr<UWidgetInteractionComponent> GetWidgetInteractionRight() const { return *WidgetInteractionRight; }
	TObjectPtr<UWidgetComponent> GetWidgetComponent() const { return *WidgetComponent; }

	UFUNCTION()
	void SetVisibleRazerMesh(bool visible);

	UPROPERTY()
	FOnPlayerDeath OnPlayerDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:


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

	bool bIsActive = true;
	bool bMouseClickEnable = true;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<APlayerWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<UInputManager> InputManager;

	//////// VR

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMotionControllerComponent> MotionControllerLeft;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractionLeft;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMotionControllerComponent> MotionControllerRight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractionRight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> VRCamera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MotionControllerLeftLazerMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MotionControllerRightLazerMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;
};
