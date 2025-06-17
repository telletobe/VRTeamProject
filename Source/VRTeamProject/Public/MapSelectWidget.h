// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelectWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionSelected, FName, RegionID);

class UButton;
class UStageInfoWidget;

/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UMapSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY() 
	FOnRegionSelected OnRegionSelected;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_KyoungGi;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> SelectButton_2;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> SelectButton_3;

	UFUNCTION()
	void HandleRegionClicked();

};
