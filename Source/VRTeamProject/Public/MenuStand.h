// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/WidgetComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MenuStand.generated.h"


UCLASS()
class VRTEAMPROJECT_API AMenuStand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuStand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:


};
