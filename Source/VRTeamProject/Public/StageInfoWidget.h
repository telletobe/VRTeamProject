// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageInfoWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartStage, FName, RegionID);

class UButton;
class UImage;
class UTextBlock;



/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UStageInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnStartStage OnStartStage;

	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void BackToMenu();
	UFUNCTION()
	void OnLevelLoaded();
	UFUNCTION(BlueprintCallable)
	void Init(const FName& InRegionID, UTexture2D* Thumbnail);

	//¸Ê Á¤º¸
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IMG_Thumb;
	
	

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_GameStart;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Back;

	FName RegionID;

};
