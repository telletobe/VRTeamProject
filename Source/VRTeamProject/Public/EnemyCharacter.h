// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AAIController;
class UNavigationInvokerComponent;


UCLASS()
class VRTEAMPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// NavInvoker Subobject return forcely
	FORCEINLINE class UNavigationInvokerComponent* GetNavInvoker() const { return NavInvoker; }

	void SetHp(float EnemyHp);
	void SetDef(float EnemyDef);
	void SetAtk(float EnemyAtk);

	float GetHp() { return Hp; }
	float GetDef() { return Def; }
	float GetAtk() { return Atk; }

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

	float NavGenerationRadius; // 동적으로 초록색 칸을 생성
	float NavRemovalRadius; // 동적으로 초록색 칸을 제거

	UPROPERTY(EditAnywhere)
	float Hp;

	UPROPERTY(EditAnywhere)
	float Def;

	UPROPERTY(EditAnywhere)
	float Atk;

};
