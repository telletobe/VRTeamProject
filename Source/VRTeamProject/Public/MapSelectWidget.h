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

/*
������ ���� ��ȣ
���� ���� 108
��� ���� 119
������ ��õ 101
��� û�� 131
�泲 ȫ�� 129
���� ���� 146
���� 165
��� ���� 138
�泲 â�� 155
���� 184
�λ� 159
���� 156
���� 133
��� 152
��õ 112
*/

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_KyoungGi; //119

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Jeonju; //146

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Kangwon; //101

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_cheongju; //131

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Hongseong; //129

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_daejeon; //133

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Pohang; //138

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Changwon; //155

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Busan; //159

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button_Seoul; //108 special Map

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStageInfoWidget> WBP_StageInfoWidget;

	UFUNCTION()
	void OnClickKyounGi();

	UFUNCTION()
	void OnClickJeonju();
	UFUNCTION()
	void OnClickKangwon();

	UFUNCTION()
	void OnClickcheongju();

	UFUNCTION()
	void OnClickHongseong();

	UFUNCTION()
	void OnClickdaejeon();

	UFUNCTION()
	void OnClickPohang();

	UFUNCTION()
	void OnClickChangwon();

	UFUNCTION()
	void OnClickBusan();

	UFUNCTION()
	void OnClickSeoul();

};
