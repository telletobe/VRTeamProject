// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UMapSelectWidget;
class UPlayerStateWidget;
class APlayerController;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public :
	APlayerHUD();

	UFUNCTION(BlueprintCallable)
	void ToggleMapSelect();

	UFUNCTION(BlueprintCallable)
	void PlayerStateShow();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMapSelectWidget> MapSelect;
	UPROPERTY(Transient)
	TObjectPtr<UMapSelectWidget> MapSelectInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerStateWidget> PlayerState;
	UPROPERTY(Transient)
	TObjectPtr<UPlayerStateWidget> PlayerStateInstance;

	UPROPERTY()
	TObjectPtr<APlayerController> PC;

	FInputModeGameAndUI Mode;
};
