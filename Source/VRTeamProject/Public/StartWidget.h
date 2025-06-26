// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> BT_StartButton;

	
};
