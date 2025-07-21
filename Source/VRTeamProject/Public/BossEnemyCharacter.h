// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "BossEnemyCharacter.generated.h"

UCLASS()
class VRTEAMPROJECT_API ABossEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	ABossEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	// Tick에서 체력 체크 및 공격 로직
	virtual void Tick(float DeltaTime) override;

private:
	// 돌 던지기 공격 몽타주
	UPROPERTY(EditAnywhere, Category = "Boss|Animation")
	TObjectPtr<UAnimMontage> ThrowMontage;

	// 풍차 공격 몽타주
	UPROPERTY(EditAnywhere, Category = "Boss|Animation")
	TObjectPtr<UAnimMontage> SpinMontage;

	// 공격 중복 방지
	bool bIsAttacking = false;

	// 던지는 로직 함수
	void PerformThrowAttack();

	// 풍차치기 로직 함수
	void PerformSpinAttack();

	// 실제 발사 시점 지연용
	void LaunchRock();

	void SetIsThrowing(bool bThrowing);


	FTimerHandle AttackCooldownHandle;
	FTimerHandle LaunchRockHandle;
	FTimerHandle DelayLaunchSetHandle;
	FTimerHandle LaunchRockTimerHandle;

protected:
	// 던질 탄환 클래스 (에디터에서 할당)
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<class AEnemyBullet> EnemyBulletClass;

	UFUNCTION(BlueprintCallable, Category = "Boss|Notify")
	void NotifyThrowProjectile();

};
