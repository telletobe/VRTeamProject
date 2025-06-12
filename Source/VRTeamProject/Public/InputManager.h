// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputManager.generated.h"

class UInputMappingContext;
class UInputAction;
class APlayerCharacter;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UInputManager : public UObject
{
	GENERATED_BODY()

private:
	static UInputManager* Instance;
	UInputManager();


public:
	static UInputManager* GetInstance();
	void Initialize(APlayerCharacter* PlayerCharacter, APlayerController* PC);
	void BindAction(UEnhancedInputComponent* InputComponent);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void ToggleMap(const FInputActionValue& Value);
	void PlayerStat(const FInputActionValue& Value);
	void Click(const FInputActionValue& Value);

private:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> IMC_InputMappingContext;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_ToggleMap;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_PlayerStat;

	UPROPERTY()
	TObjectPtr<UInputAction> IA_Click;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;



};
