// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameItem.generated.h"

class UBoxComponent;
class APlayerCharacter;
class ATargetPoint;

UENUM(BlueprintType)
enum class EItemEffectData : uint8
{
	AtkUp			UMETA(DisplayName = "AtkUp"),
	AttackSpeed	UMETA(DisplayName = "AttackSpeed"),
	DefUp			UMETA(DisplayName = "DefUp"),
	HEAL			UMETA(DisplayName = "HEAL")

};


UCLASS()
class VRTEAMPROJECT_API AGameItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameItem();

	void SetHp(float ItemHp);
	void SetItemData(EItemEffectData EItemData);

	float GetHp() const { return Hp; }
	EItemEffectData GetItemData() const { return ItemData; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse,	const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:


	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> ItemCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ParachuteMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BoxTopMesh;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;

	UPROPERTY(EditAnywhere)
	EItemEffectData ItemData;

	UPROPERTY(EditAnywhere)
	float Hp = 25.0f;

	UPROPERTY(VisibleAnywhere)
	float MaxHp = 25.0f;


};
