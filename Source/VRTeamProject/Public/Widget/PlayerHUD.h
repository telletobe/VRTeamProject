// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UMapSelectWidget;
class UPlayerStateWidget;
class UStageInfoWidget;
class UEndGameWidget;
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

	TObjectPtr<UMapSelectWidget> GetMapSelectInstance() const;
	TObjectPtr<UPlayerStateWidget> GetPlayerStateInstance() const;
	TObjectPtr<UStageInfoWidget> GetStageInfoInstance() const;
	TObjectPtr<UEndGameWidget> GetEndGameInstance() const;

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UStageInfoWidget> StageInfo;
	UPROPERTY(Transient)
	TObjectPtr<UStageInfoWidget> StageInfoInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UEndGameWidget> EndGame;

	UPROPERTY(Transient)
	TObjectPtr<UEndGameWidget> EndGameInstance;

	UPROPERTY()
	TObjectPtr<APlayerController> PC;

	FInputModeGameAndUI Mode;
};
