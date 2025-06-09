// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateWidget.generated.h"

class UProgressBar;
class UButton;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void UpdateHP();


protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_0;


};
