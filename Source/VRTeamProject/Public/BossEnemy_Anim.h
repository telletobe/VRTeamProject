// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossEnemy_Anim.generated.h"

/**
 * 
 */
UCLASS()
class VRTEAMPROJECT_API UBossEnemy_Anim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// ���� ���� ���� ��ȯ ����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AnimState")
	bool bIsThrowing = false;
	
};
