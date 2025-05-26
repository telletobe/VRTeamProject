// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "Logging/LogMacros.h"
#include "NavigationInvokerComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class VRTEAMPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// NavInvoker Subobject return forcely
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return NavInvoker; }


	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	void MoveToTarget();
	void StartMoving();
	void FindTargetPoints();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private:
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	//Invoker setting
	UPROPERTY(EditAnywhere, Category = Navigation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNavigationInvokerComponent> NavInvoker;

	UPROPERTY(EditAnywhere, Category = "AI MoveMent")
	TObjectPtr<AActor> Target;

	UPROPERTY(EditAnywhere, Category = "AI MoveMent")
	TObjectPtr<AActor> Target2;

	bool bIsSucceeded;
	bool bIsMoving;

	float NavGenerationRadius; // 동적으로 초록색 칸을 생성
	float NavRemovalRadius; // 동적으로 초록색 칸을 제거
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere)
	float Hp;

	UPROPERTY(EditAnywhere)
	float Def;

	UPROPERTY(EditAnywhere)
	float Atk;

};
