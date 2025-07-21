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
	// 공격 루프 상태 전환 변수
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AnimState")
	bool bIsThrowing = false;
	
};
