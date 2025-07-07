// Fill out your copyright notice in the Description page of Project Settings.


#include "VRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include <EnemyCharacter.h>
#include <EnemySpawner.h>
#include <ItemSpawnActor.h>
#include <WeatherManager.h>

/*
OnRestart.AddUniqueDynamic(PlayerActor, &APlayerCharacter::InVisibleRezerMesh);
OnRestart.AddUniqueDynamic(PlayerActor, &APlayerCharacter::PlayerReSpawn);
PlayerActor->OnPlayerDeath.AddDynamic(this, &AVRProjectGameModeBase::OnPlayerDeath);
PlayerActor->OnPlayerDeath.AddDynamic(this, &AVRProjectGameModeBase::CleanupGameItem);
OnPlayerDied.AddDynamic(this,&AVRProjectGameModeBase::DeActivateEnemySpawner);
Spanwer->OnEnemySpawned.AddDynamic(this,&AVRProjectGameModeBase::OnEnemySpawned);
SpawnedEnemy->OnEnemyKilled.AddDynamic(this, &AVRProjectGameModeBase::CheckGameClear);
*/


AVRProjectGameModeBase::AVRProjectGameModeBase()
{

	bIsClear = false;
		
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = APlayerHUD::StaticClass();

	CurrentKillCnt = 0;
	RequiredKillCnt = 20;
}

void AVRProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActor);

	for (AActor* AllActor : FoundActor)
	{
		if (APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(AllActor))
		{
			if (PlayerActor)
			{
				OnRestart.AddUniqueDynamic(PlayerActor, &APlayerCharacter::PlayerReSpawn);
				PlayerActor->OnPlayerDeath.AddDynamic(this, &AVRProjectGameModeBase::OnPlayerDeath);
				PlayerActor->OnPlayerDeath.AddDynamic(this, &AVRProjectGameModeBase::CleanupGameItem);
			}
		}
	}
}


void AVRProjectGameModeBase::TriggerGameClear()
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::MakeRandomColor(), TEXT("Clear Game"));
	bIsClear = true;
	CleanupAfterGameEnd();
	return;
}

void AVRProjectGameModeBase::TriggerGameStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("Start Game"));
	bIsClear = false;
	bPlayerAlive = true;

	InitializeGameObjects();
	CleanupGameItem();

	// 플레이어가 죽으면 적 생성을 멈추는 델리게이트 설정
	if (Spanwer)
	{
		OnPlayerDied.AddDynamic(this,&AVRProjectGameModeBase::DeActivateEnemySpawner);
	}

	return;
}

void AVRProjectGameModeBase::TriggerGameReStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("ReStart Game"));

	bIsClear = false;
	bPlayerAlive = true;

	CleanupGameItem();
	
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActor);

	for (AActor* AllActor : FoundActor)
	{
		if (APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(AllActor))
		{
			PlayerActor->PlayerReSpawn();
		}
	}

	if (Spanwer) 
	{
		Spanwer->ActivateEnemySpawner();
	}

	return;
}

void AVRProjectGameModeBase::CleanupAfterGameEnd()
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActor);

	for (AActor* AllActor : FoundActor)
	{
		if (APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(AllActor))
		{
			PlayerActor->SetHp(PlayerActor->GetMaxHp());
			continue;
		}
		else if (AItemSpawnActor* ItemSpanwer = Cast<AItemSpawnActor>(AllActor))
		{
			if (IsValid(ItemSpanwer))
			{
				ItemSpanwer->Destroy();
				bItemSpawnerExists = false;
			}
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
					Enemy->OnEnemyKilled.RemoveDynamic(this,&AVRProjectGameModeBase::CheckGameClear); //설정 해둔 델리게이트 삭제
					Enemy->Destroy();
				}
			}
			if (IsValid(EnemySpanwer))
			{
				EnemySpanwer->OnEnemySpawned.RemoveDynamic(this, &AVRProjectGameModeBase::OnEnemySpawned);  //설정 해둔 델리게이트 삭제
				EnemySpanwer->Destroy();
				bEnemySpawnerExists = false;
			}
		}
	}
}

void AVRProjectGameModeBase::CleanupGameItem() // �÷��̾� ����� �ʵ忡�ִ� ������ �޸� �Ҵ�����
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
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActor);

	for (AActor* Spawner : FoundActor)
	{
		AEnemySpawner* ExsitSpawner = Cast<AEnemySpawner>(Spawner);
		if (IsValid(ExsitSpawner))
		{
			bEnemySpawnerExists = true;
			break;
		}
	}

	for (AActor* Spawner : FoundActor)
	{
		AItemSpawnActor* ExsitSpawner = Cast<AItemSpawnActor>(Spawner);
		if (IsValid(ExsitSpawner))
		{
			bItemSpawnerExists = true;
			break;
		}
	}

	if (BPEnemySpawner)
	{
		if (!bEnemySpawnerExists)
		{
			if (!Spanwer)
			{
				Spanwer = GetWorld()->SpawnActor<AEnemySpawner>(BPEnemySpawner, FVector(FVector::ZeroVector), FRotator(0, 0, 0));
				Spanwer->CreateEnemy();
				GetWorldTimerManager().SetTimer(Spanwer->GetSpawnHandle(), Spanwer.Get(), &AEnemySpawner::SpawnEnemy, Spanwer->GetSpawnDelay(), true);
				Spanwer->OnEnemySpawned.AddDynamic(this,&AVRProjectGameModeBase::OnEnemySpawned);

			}			
			bEnemySpawnerExists = true;
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Make sure to assign this in the editor before running the game."));
	}

	if (BPItemSpawner)
	{
		if (!bItemSpawnerExists)
		{
			AItemSpawnActor* ItemSpawner = GetWorld()->SpawnActor<AItemSpawnActor>(BPItemSpawner, FVector(0, 0, 0), FRotator(0, 90.0f, 0));
			bItemSpawnerExists = true;
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Make sure to assign this in the editor before running the game."));
	}
}

void AVRProjectGameModeBase::CheckGameClear()
{
	CurrentKillCnt++;
	UE_LOG(LogTemp,Warning,TEXT("call CheckGameClear"));
	if (RequiredKillCnt <= CurrentKillCnt)
	{
		TriggerGameClear();
	}
	else
	{
		return;
	}
}

void AVRProjectGameModeBase::OnEnemySpawned(AEnemyCharacter* SpawnedEnemy)
{
	SpawnedEnemy->OnEnemyKilled.AddDynamic(this, &AVRProjectGameModeBase::CheckGameClear);
}

void AVRProjectGameModeBase::OnPlayerDeath()
{
	bPlayerAlive = false;
	OnPlayerDied.Broadcast();
	return;
}

void AVRProjectGameModeBase::NotifyReStart()
{
	OnRestart.Broadcast();
}

void AVRProjectGameModeBase::DeActivateEnemySpawner()
{
	if (Spanwer)
	{
		Spanwer->DeActivateEnemySpawner();
	}
}
