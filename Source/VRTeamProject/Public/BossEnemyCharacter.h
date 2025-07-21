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
	// Tick���� ü�� üũ �� ���� ����
	virtual void Tick(float DeltaTime) override;

private:
	// �� ������ ���� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = "Boss|Animation")
	TObjectPtr<UAnimMontage> ThrowMontage;

	// ǳ�� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, Category = "Boss|Animation")
	TObjectPtr<UAnimMontage> SpinMontage;

	// ���� �ߺ� ����
	bool bIsAttacking = false;

	// ������ ���� �Լ�
	void PerformThrowAttack();

	// ǳ��ġ�� ���� �Լ�
	void PerformSpinAttack();

	// ���� �߻� ���� ������
	void LaunchRock();

	void SetIsThrowing(bool bThrowing);


	FTimerHandle AttackCooldownHandle;
	FTimerHandle LaunchRockHandle;
	FTimerHandle DelayLaunchSetHandle;
	FTimerHandle LaunchRockTimerHandle;

protected:
	// ���� źȯ Ŭ���� (�����Ϳ��� �Ҵ�)
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<class AEnemyBullet> EnemyBulletClass;

	UFUNCTION(BlueprintCallable, Category = "Boss|Notify")
	void NotifyThrowProjectile();

};
