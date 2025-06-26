// Fill out your copyright notice in the Description page of Project Settings.


#include "StageInfoWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>

void UStageInfoWidget::Init(const FName& InRegionID, UTexture2D* Thumbnail, int32 Difficulty)
{
   /* RegionID = InRegionID;
    if (TXT_Title)      
        TXT_Title->SetText(FText::FromName(RegionID));
    if (IMG_Thumb)      
        IMG_Thumb->SetBrushFromTexture(Thumb);
    if (TXT_Difficulty) 
        TXT_Difficulty->SetText(FText::AsNumber(Diff));*/
}

void UStageInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_GameStart)
    {
        Btn_GameStart->OnClicked.AddUniqueDynamic(this, &UStageInfoWidget::GameStart);
    }
    if (Btn_Back)
    {
        Btn_Back->OnClicked.AddUniqueDynamic(this, &UStageInfoWidget::BackToMenu);
    }
}

void UStageInfoWidget::GameStart()
{

    FLatentActionInfo Latent;
    Latent.CallbackTarget = this;
    Latent.ExecutionFunction = "OnStreamLevelLoaded";
    Latent.Linkage = 0;
    Latent.UUID = __LINE__;
    UGameplayStatics::LoadStreamLevel(this, FName(TEXT("FirstMap")), true, true, Latent);
}

void UStageInfoWidget::BackToMenu()
{
}
