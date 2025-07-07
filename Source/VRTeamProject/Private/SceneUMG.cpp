// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneUMG.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TimerManager.h"
#include "Engine/Texture2D.h"

void USceneUMG::NativeConstruct()
{
    // 버튼 숨기기
    if (BT_NextRound)
    {
        BT_NextRound->SetVisibility(ESlateVisibility::Hidden);
    }


    // 텍스처 로딩
    UTexture2D* Texture1 = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UMG/Image/ZombieFaceImage1.ZombieFaceImage1'"));
    UTexture2D* Texture2 = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/UMG/Image/ZombieFaceImage2.ZombieFaceImage2'"));

    if (Texture1 && ZombieFaceImage1)
    {
        FSlateBrush Brush1;
        Brush1.SetResourceObject(Texture1);
        Brush1.ImageSize = FVector2D(256, 256);
        ZombieFaceImage1->SetBrush(Brush1);
    }

    if (Texture2 && ZombieFaceImage2)
    {
        FSlateBrush Brush2;
        Brush2.SetResourceObject(Texture2);
        Brush2.ImageSize = FVector2D(256, 256);
        ZombieFaceImage2->SetBrush(Brush2);
    }

    // 대사 설정: TPair<화자인덱스, 텍스트>
    DialogueLines = {
        TPair<int32, FString>(1, TEXT("야, 센다이 쪽 지진으로 원전 터진 거 알지?")),
        TPair<int32, FString>(2, TEXT("그 옆에 연구소에서 이상한 샘플이 흘러나왔다던데?")),
        TPair<int32, FString>(1, TEXT("그래서 일본이 먼저 대환장파티가 진행중이래.")),
        TPair<int32, FString>(2, TEXT("우리 쪽도 금세 퍼지는거 아닌가?")),
        TPair<int32, FString>(1, TEXT("설마 코로나때처럼 또 그러는거 아니겠지?"))
    };

    CurrentLineIndex = 0;
    StartNextDialogue(); // 첫 줄 시작

    if (!Round1BGM)
    {
        Round1BGM = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Audio/BGM/Round1.Round1'"));
    }

    // BGM 재생
    if (Round1BGM)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), Round1BGM);
    }

}

void USceneUMG::StartNextDialogue()
{
    if (DialogueLines.IsValidIndex(CurrentLineIndex))
    {
        int32 Speaker = DialogueLines[CurrentLineIndex].Key;
        FullText = DialogueLines[CurrentLineIndex].Value;
        CurrentText.Empty();
        CurrentCharIndex = 0;

        // 해당 화자 텍스트만 보이게 설정
        if (Speaker == 1)
        {
            if (ZombieTalk1) ZombieTalk1->SetText(FText::GetEmpty());
            if (ZombieTalk2) ZombieTalk2->SetText(FText::GetEmpty());
        }
        else
        {
            if (ZombieTalk1) ZombieTalk1->SetText(FText::GetEmpty());
            if (ZombieTalk2) ZombieTalk2->SetText(FText::GetEmpty());
        }

        GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &USceneUMG::TypeNextCharacter, 0.1f, true);
    }
    else
    {
        // 대사 종료 후 버튼 노출
        if (BT_NextRound)
        {
            BT_NextRound->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Log, TEXT("모든 대사 출력 완료"));
    }

}

void USceneUMG::TypeNextCharacter()
{

    if (CurrentCharIndex < FullText.Len())
    {
        CurrentText.AppendChar(FullText[CurrentCharIndex]);

        // 현재 화자에 따라 다른 텍스트 블록에 출력
        int32 Speaker = DialogueLines[CurrentLineIndex].Key;
        if (Speaker == 1 && ZombieTalk1)
        {
            ZombieTalk1->SetText(FText::FromString(CurrentText));
        }
        else if (Speaker == 2 && ZombieTalk2)
        {
            ZombieTalk2->SetText(FText::FromString(CurrentText));
        }

        CurrentCharIndex++;
    }
    else
    {
        // 한 줄 끝나면 다음 대사로
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        FTimerHandle DelayHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DelayHandle,
            FTimerDelegate::CreateLambda([this]()
                {
                    CurrentLineIndex++;
                    StartNextDialogue();
                }),
            1.0f, false
        );
    }

}



