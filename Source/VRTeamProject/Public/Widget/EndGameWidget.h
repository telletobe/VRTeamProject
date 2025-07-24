// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWidget.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	UFUNCTION()
	void ShowEndGame();

protected:
	virtual void NativeConstruct() override;

private:
	
};
