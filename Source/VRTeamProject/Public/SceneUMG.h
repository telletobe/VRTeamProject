// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundBase.h" // UPROPERTY용
#include "Kismet/GameplayStatics.h" // 재생용
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SceneUMG.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API USceneUMG : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (Bindwidget))
	UImage* ZombieFaceImage1;

	UPROPERTY(meta = (Bindwidget))
	UImage* ZombieFaceImage2;


    UPROPERTY(meta = (BindWidget))
    UTextBlock* ZombieTalk1;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ZombieTalk2;

    UPROPERTY(meta = (Bindwidget))
    TObjectPtr<UButton> BT_NextRound;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundBase* Round1BGM;

    // 여러 줄 대사 처리용
    TArray<TPair<int32, FString>> DialogueLines;
    int32 CurrentLineIndex = 0;

    // 현재 출력 중인 텍스트 정보
    FString FullText;
    FString CurrentText;
    int32 CurrentCharIndex = 0;

    FTimerHandle TypingTimerHandle;

   

    void TypeNextCharacter(); // 한 글자씩 출력
    // void StartTyping(const FString& TextToType, float Interval = 0.05f); // 문자열 타이핑 시작
    void StartNextDialogue(); // 다음 줄 대사 시작

};
