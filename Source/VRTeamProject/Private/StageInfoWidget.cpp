// Fill out your copyright notice in the Description page of Project Settings.


#include "StageInfoWidget.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>
#include <VRProjectGameModeBase.h>
#include <PlayerCharacter.h>

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
    AVRProjectGameModeBase* GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        if (GameMode->IsClear() == false)
        {
            return;
        }
        else
        {        
            // 스트리밍 방식으로 로드
            FLatentActionInfo LatentInfo;
            LatentInfo.CallbackTarget = this;
            LatentInfo.ExecutionFunction = "OnLevelLoaded";
            LatentInfo.Linkage = 0;
            LatentInfo.UUID = __LINE__;

            UGameplayStatics::LoadStreamLevel(this, FName("M_Basic"), true, false, LatentInfo);
            UE_LOG(LogTemp, Warning, TEXT("LoadStreamLevel"));
        }
    }
}

void UStageInfoWidget::BackToMenu()
{
    SetVisibility(ESlateVisibility::Hidden);
}

void UStageInfoWidget::OnLevelLoaded()
{
    AVRProjectGameModeBase* GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->TriggerGameStart();
    }

   /* APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (IsValid(Player))
    {
        Player->SpawnWeapon();
        UE_LOG(LogTemp, Log, TEXT("Weapon player."));
    }*/

}
