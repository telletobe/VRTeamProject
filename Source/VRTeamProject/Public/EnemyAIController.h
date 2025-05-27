// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Chasing
};

UCLASS()
class VRTEAMPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


public:
	AEnemyAIController();

	void StartChasing(AActor* Target);
	void StopChasing();
	void UpdateAIState();

	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	EAIState GetCurrentState() const { return CurrentState; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	EAIState CurrentState = EAIState::Idle;

	bool bIsChasing = false;

	UPROPERTY(EditAnywhere, Category = "AI")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRadius = 4000.0f; //�����Ÿ�

	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseInterestRadius = 6000.0f; // ��ǥ�� �Ҵ� �Ÿ�(�����ϴ� �Ÿ����� Ŀ����

	UPROPERTY(EditAnywhere, Category = "AI")
	float ChaseRadius = 5000.f; //�i�°Ÿ�



};
