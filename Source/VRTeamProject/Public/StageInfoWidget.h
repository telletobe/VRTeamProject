// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageInfoWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartStage, FName, RegionID);

class UButton;



/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UStageInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(const FName& InRegionID, UTexture2D* Thumbnail, int32 Difficulty);

	UPROPERTY(BlueprintAssignable)
	FOnStartStage OnStartStage;

	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void BackToMenu();
	UFUNCTION()
	void OnLevelLoaded();
	

protected:
	virtual void NativeConstruct() override;

private:

	//¸Ê Á¤º¸
	//UPROPERTY(meta = (BindWidget))
	//class UTextBlock* TXT_Title;
	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UImage> IMG_Thumb;
	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> TXT_Difficulty;


	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Btn_GameStart;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Btn_Back;

	FName RegionID;


	
};
