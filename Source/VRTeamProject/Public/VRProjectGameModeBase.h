// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API AVRProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	AVRProjectGameModeBase();
public :
	void TriggerGameClear();
	void TriggerGameStart();
	bool IsClear() { return bIsClear; }
	bool IsPlayerAlive() { return bPlayerAlive; }

	UFUNCTION()
	void ChangeIsPlayerDead();
protected:

	virtual void BeginPlay() override;

private:
	bool bIsClear = false;
	bool bPlayerAlive = true;
};
