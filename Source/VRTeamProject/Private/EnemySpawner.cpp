// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include <EnemyCharacter.h>

float AEnemySpawner::SpawnDelay = 1.0f;

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
	bool bIsSpawn = true; 
	////////////////////////////////////////////////////////////////////////////////
	// 임시 코드

	if(bIsSpawn)
	{
		for (AEnemyCharacter* Enemy : EnemyPool)
		{
			if (IsValid(Enemy) && !(Enemy->IsActive()))
			{
				Enemy->Spawn();
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////
}

void AEnemySpawner::CreateEnemy()
{
	FVector SpawnPoint = FMath::RandPointInBox(SpawnBox->Bounds.GetBox());
	////////////////////////////////////////////////////////////////////////////////
	// 임시 코드

	bool bIsSpawn = true;

	if (bIsSpawn)
	{
		TSubclassOf<AEnemyCharacter> CommonEnemy = LoadClass<AEnemyCharacter>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Character/Enemy/BP_EnemyCharacter.BP_EnemyCharacter_C'"));
		if (CommonEnemy)
		{

			if (EnemyPool.Num() < EnemyPooSize)
			{
				AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(CommonEnemy, SpawnPoint, FRotator(0));

				if (SpawnedEnemy)
				{
					SpawnedEnemy->SpawnDefaultController();
				}

				EnemyPool.Add(SpawnedEnemy);

			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(CreateHandle);
				GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Cyan,TEXT("Fin"));
			}



		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("InValid Enemy!"));
		}
	}
}


// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	float RandomOffset = FMath::RandRange(0.0f, SpawnDelay);

	GetWorld()->GetTimerManager().SetTimer(CreateHandle,this,&AEnemySpawner::CreateEnemy,CreateDelay,true);
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SpawnEnemy();

}

