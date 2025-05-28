// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputMappingContext.h>
#include "PlayerCharacter.generated.h"

class APlayerWeapon;
class APlayerController;

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


	///////////////////////////////////////////////////
	// 미사용 코드
	//void MoveTargetPoint(AActor* TargetPoint);
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
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

	UPROPERTY(EditAnywhere)
	float Atk;

	UPROPERTY(EditAnywhere)
	float Def;

	UPROPERTY(VisibleAnywhere)
	float Exp;

	bool bIsArrived;
	
	UPROPERTY()
	TObjectPtr<UInputMappingContext> IMC_InputMappingContext;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> EndPoint;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<APlayerWeapon> Weapon;
	
};
