// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateWidget.generated.h"

class UProgressBar;
class UButton;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void UpdatePlayerHP(float CurrentHp,float MaxHp);

	UFUNCTION()
	void ReSetHp();

	void UpdatePlayerDef();
	void UpdatePlayerExp(float CurrentExp);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> DefBar;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> ExpBar;

	


};