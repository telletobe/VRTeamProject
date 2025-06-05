// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UMapSelectWidget;
class UPlayerStateWidget;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public :
	APlayerHUD();
protected:
	virtual void BeginPlay() override;
private:
	TSubclassOf<UMapSelectWidget> MapSelect;

	TObjectPtr<UMapSelectWidget> MapSelectInstance;

	TSubclassOf<UPlayerStateWidget> PlayerState;

	TObjectPtr<UPlayerStateWidget> PlayerStateInstance;
};
