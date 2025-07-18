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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Region")
	TMap<FName, UTexture2D*> RegionThumbnails;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_KyoungGi; //119

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Jeonju; //146

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Kangwon; //101

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStageInfoWidget> WBP_StageInfoWidget;

	UFUNCTION()
	void OnClickKyounGi();

	UFUNCTION()
	void OnClickJeonju();
	UFUNCTION()
	void OnClickKangwon();
};
