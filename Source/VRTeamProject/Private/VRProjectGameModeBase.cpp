// Fill out your copyright notice in the Description page of Project Settings.


#include "VRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include <EnemyCharacter.h>
#include <EnemySpawner.h>
#include <ItemSpawnActor.h>
#include <WeatherManager.h>

AVRProjectGameModeBase::AVRProjectGameModeBase()
{

	bIsClear = false;
		
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	
}

void AVRProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TriggerGameStart();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		Player->OnPlayerDeath.AddDynamic(this, &AVRProjectGameModeBase::ChangePlayerAliveState);
		Player->OnPlayerDeath.AddDynamic(this, &AVRProjectGameModeBase::CleanupGameItem);
	}
}


void AVRProjectGameModeBase::TriggerGameClear()
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::MakeRandomColor(), TEXT("Clear Game"));
	bIsClear = true;
	CleanupAfterGameClear();
	return;
}

void AVRProjectGameModeBase::TriggerGameStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("Start Game"));
	bIsClear = false;
	bPlayerAlive = true;

	InitializeGameObjects();
	CleanupGameItem();
	return;
}

void AVRProjectGameModeBase::TriggerGameReStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("ReStart Game"));
	bIsClear = false;
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		Player->PlayerReSpawn();
		bPlayerAlive = true;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player Is Destroyed"));
		return;
	}

	CleanupGameItem();
	NotifyReStart();
	return;
}

void AVRProjectGameModeBase::CleanupAfterGameClear()
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActor);

	for (AActor* AllActor : FoundActor)
	{
		if (const APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(AllActor))
		{
			continue;
		}
		else if (AItemSpawnActor* ItemSpanwer = Cast<AItemSpawnActor>(AllActor))
		{
			if (IsValid(ItemSpanwer)) ItemSpanwer->Destroy();
		}
		else if (AGameItem* GameItem = Cast<AGameItem>(AllActor))
		{
			if (IsValid(GameItem))
			{
				GameItem->Destroy();
			}
		}
		else if (AEnemySpawner* EnemySpanwer = Cast<AEnemySpawner>(AllActor)) {
			TArray<AEnemyCharacter*> EnemyPool = EnemySpanwer->GetEnemyPool();
			for (auto Enemy : EnemyPool)
			{
				if (IsValid(Enemy))
				{
					Enemy->OnEnemyDespawned.RemoveDynamic(EnemySpanwer, &AEnemySpawner::CheckGameClear);
					Enemy->OnEnemyDeath.RemoveDynamic(EnemySpanwer, &AEnemySpawner::IncreaseKillCount);
					Enemy->Destroy();
				}
			}
			if (IsValid(EnemySpanwer)) EnemySpanwer->Destroy();
		}
	}
}

void AVRProjectGameModeBase::CleanupGameItem()
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActor);

	for (AActor* AllActor : FoundActor)
	{
		if (AGameItem* GameItem = Cast<AGameItem>(AllActor))
		{
			if (IsValid(GameItem))
			{
				GameItem->Destroy();
			}
		}
	}
}

void AVRProjectGameModeBase::InitializeGameObjects()
{

	if (!WeatherManager)
	{
		WeatherManager = UWeatherManager::GetInstance();
		if (WeatherManager)
		{
			WeatherManager->SetCurrentWorld(GetWorld());
			WeatherManager->Init();
		}
	}

	if (BPEnemySpawner)
	{
		AEnemySpawner* Spanwer = GetWorld()->SpawnActor<AEnemySpawner>(BPEnemySpawner, FVector(-1350.0f, 3200.0f, 350.0f), FRotator(0, 0, 0));
	}

	if (BPItemSpawner)
	{
		AItemSpawnActor* ItemSpawner = GetWorld()->SpawnActor<AItemSpawnActor>(BPItemSpawner, FVector(0, 0, 0), FRotator(0, 90.0f, 0));
	}
}

void AVRProjectGameModeBase::ChangePlayerAliveState()
{
	bPlayerAlive = !bPlayerAlive;
	UE_LOG(LogTemp,Warning,TEXT("PlayerAlive : %s"), bPlayerAlive ? TEXT("true") : TEXT("false"));
	return;
}

void AVRProjectGameModeBase::NotifyReStart()
{
	OnRestart.Broadcast();
}
