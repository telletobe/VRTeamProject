// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelectWidget.generated.h"

class UButton;
	

/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UMapSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> SelectButton_1;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> SelectButton_2;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> SelectButton_3;

};
