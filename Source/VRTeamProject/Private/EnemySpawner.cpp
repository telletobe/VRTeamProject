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

			if (EnemyPool.Num() < 20)
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
	
	GetWorld()->GetTimerManager().SetTimer(CreateHandle,this,&AEnemySpawner::CreateEnemy,0.1f,true);
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, 1.0f, true);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SpawnEnemy();

}

