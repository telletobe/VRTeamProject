// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> BT_StartButton;

	
};
