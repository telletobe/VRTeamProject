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
	
	UFUNCTION()
	void ReStart();

	UFUNCTION()
	void Quit();
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> BT_RestartButton;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> BT_QuitButton;

	

	
	
};
