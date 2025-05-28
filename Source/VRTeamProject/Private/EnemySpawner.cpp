// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include <EnemyCharacter.h>

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

}

void AEnemySpawner::SpawnEnemy()
{
	FVector SpawnPoint = FMath::RandPointInBox(SpawnBox->Bounds.GetBox());
	////////////////////////////////////////////////////////////////////////////////
	// 임시 코드

	bool bIsSpawn = true; 

	if(bIsSpawn)
	{
		TSubclassOf<AEnemyCharacter> CommonEnemy = LoadClass<AEnemyCharacter>(nullptr,TEXT("/Script/Engine.Blueprint'/Game/Character/Enemy/BP_EnemyCharacter.BP_EnemyCharacter_C'"));
		if (CommonEnemy)
		{
			AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(CommonEnemy,SpawnPoint, FRotator(0));
			if (SpawnedEnemy)
			{
				SpawnedEnemy->SpawnDefaultController();
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Red,TEXT("InValid Enemy!"));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle,this,&AEnemySpawner::SpawnEnemy,SpawnDelay,true);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

