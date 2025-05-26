// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_sight.h"
#include "Kismet/GamePlayStatics.h"
#include "GameFramework/Character.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */


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
	FVector LastKnownLocation;

	bool bIsChasing = false;

	UPROPERTY(EditAnywhere, Category = "AI")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRadius = 1500.0f; //감지거리

	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseInterestRadius = 2000.0f; // 목표를 잃는 거리(감지하는 거리보다 커야함

	UPROPERTY(EditAnywhere, Category = "AI")
	float ChaseRadius = 1500.f; //쫒는거리



};
