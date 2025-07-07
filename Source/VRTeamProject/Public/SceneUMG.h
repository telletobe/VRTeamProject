// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundBase.h" // UPROPERTY��
#include "Kismet/GameplayStatics.h" // �����
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

    // ���� �� ��� ó����
    TArray<TPair<int32, FString>> DialogueLines;
    int32 CurrentLineIndex = 0;

    // ���� ��� ���� �ؽ�Ʈ ����
    FString FullText;
    FString CurrentText;
    int32 CurrentCharIndex = 0;

    FTimerHandle TypingTimerHandle;

   

    void TypeNextCharacter(); // �� ���ھ� ���
    // void StartTyping(const FString& TextToType, float Interval = 0.05f); // ���ڿ� Ÿ���� ����
    void StartNextDialogue(); // ���� �� ��� ����

};
