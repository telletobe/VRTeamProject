// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include <EnemyCharacter.h>
#include "Engine/TargetPoint.h"
#include <Kismet/GameplayStatics.h>


/*
	�޸� ������ ���Ӹ�忡�� ó��
*/

const int32 AEnemySpawner::EnemyPoolSize = 20;

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

	SpawnDelay = 0.7f;
	PoolIndex = EnemyPoolSize - 1;
}

void AEnemySpawner::CreateEnemy()
{
	//�޸𸮿� Enemy�� �Ҵ��ϴ� �ڵ�
	const FVector SpawnPoint = FMath::RandPointInBox(SpawnBox->Bounds.GetBox());
	bool bIsSpawn = true;

	if (bIsSpawn)
	{
		TSubclassOf<AEnemyCharacter> CommonEnemy = LoadClass<AEnemyCharacter>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Character/Enemy/BP_EnemyCharacter.BP_EnemyCharacter_C'"));
		if (CommonEnemy)
		{
			for (int32 i = 0; i < EnemyPoolSize; i++)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(CommonEnemy, SpawnPoint, FRotator(0),SpawnParams);

				if (SpawnedEnemy)
				{
					SpawnedEnemy->SpawnDefaultController();
					SpawnedEnemy->DeSpawn();
					SpawnedEnemy->ApplyWeatherEffect(CachedWeatherData);
					EnemyPool.Add(SpawnedEnemy);
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Create Enemy"));
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("InValid Enemy!"));
		}
	}
}

void AEnemySpawner::DestroyEnemy()
{
	if (EnemyPool.Num() <= 0) return;
	for (int32 i = 0; i < EnemyPool.Num(); i++)
	{
		EnemyPool[i]->OnEnemyDie.Clear();
		EnemyPool[i]->OnEnemyDeathAnimEnded.Clear();
		EnemyPool[i]->OnEnemyAttack.Clear();
		EnemyPool[i]->Destroy();
	}
}

void AEnemySpawner::SpawnEnemy()
{
	// �޸𸮿� �Ҵ�Ǿ��ִ� Enemy�� Ȱ��ȭ ��Ű�� �ڵ�
	if (EnemyPool.Num() <= 0) return;

	for (int32 i = 0; i < EnemyPool.Num(); ++i)
	{
		int32 Index = (PoolIndex + i) % EnemyPool.Num();
		if (!EnemyPool[Index]->IsActive())
		{
			EnemyPool[Index]->Spawn();
			PoolIndex = (Index + 1) % EnemyPool.Num();
			return;
		}
	}
}

void AEnemySpawner::DeActivateEnemySpawner()
{
	//�޸𸮿� �Ҵ��� �����ϸ鼭, Enemy�� �������ʰ� ��.
	if (EnemyPool.Num() != 0)
	{
		for (AEnemyCharacter* SpawnedEnemy : EnemyPool)
		{
			if (SpawnedEnemy->IsActive())
			{
				SpawnedEnemy->DeSpawn();
			}
		}
	}
	//�������� �ʰ� Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(SpawnHandle);
}

void AEnemySpawner::ActivateEnemySpawner()
{
	//EnemySpawner�� ���� Ȱ��ȭ
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("ActivateEnemySpawner"));
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnDelay <= 0)
	{
		SpawnDelay = 0.7f;
	}
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawner::SpawnEnemy, GetSpawnDelay(), true);
	CreateEnemy();
		
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::OnWeatherChanged(EWeatherData NewWeather)
{
	CachedWeatherData = NewWeather;
}

