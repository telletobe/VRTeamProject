// Fill out your copyright notice in the Description page of Project Settings.


#include "VRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include <EnemyCharacter.h>
#include <EnemySpawner.h>
#include <ItemSpawnActor.h>
#include <WeatherManager.h>
#include "Sound/SoundCue.h"

AVRProjectGameModeBase::AVRProjectGameModeBase()
{
	bIsClear = false;
		
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = APlayerHUD::StaticClass();

	CurrentKillCnt = 0;
	RequiredKillCnt = 40;

	ConstructorHelpers::FObjectFinder<USoundCue> BGMObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/EffectSound/MainBGM.MainBGM'"));
	if (BGMObject.Succeeded())
	{
		MainBGM = BGMObject.Object;
	}
}

void AVRProjectGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (MainBGM)
	{
		UGameplayStatics::SpawnSound2D(this, MainBGM);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BGMSound Data invalid"));
	}

	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundActor);

	for (AActor* AllActor : FoundActor)
	{
		if (APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(AllActor))
		{
			if (PlayerActor)
			{
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
	CurrentKillCnt = 0;
	CleanupAfterGameEnd();
	return;
}

void AVRProjectGameModeBase::TriggerGameStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::MakeRandomColor(), TEXT("Start Game"));
	CurrentKillCnt = 0;
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	bIsClear = false;
	if (Player)
	{
		Player->PlayerReSpawn();
	}
	bPlayerAlive = true;

	//Player Replace to Start
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter)
	{
		FVector CurrentLocation = PlayerCharacter->GetActorLocation();

		FVector NewLocation = FVector(-4500.0, CurrentLocation.Y, CurrentLocation.Z); // 이동할 위치

		PlayerCharacter->SetActorLocation(NewLocation);
	}

	CleanupGameItem();
	InitializeGameObjects();

	return;
}

//게임 클리어로 인한 종료시 플레이어의 HP회복, 사용하지않는 데이터 메모리 해제
void AVRProjectGameModeBase::CleanupAfterGameEnd()
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundActor);

	for (AActor* AllActor : FoundActor)
	{
		if (APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(AllActor))
		{
			PlayerActor->SetHp(PlayerActor->GetMaxHp());
			
		}
	}
	CleanupItemSpanwer();
	CleanupEnemySpawner();
	CleanupGameItem();

	//Player Replace to Start
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (PlayerCharacter)
	{
		FVector CurrentLocation = PlayerCharacter->GetActorLocation();

		FVector NewLocation = FVector(-9000.0, CurrentLocation.Y, CurrentLocation.Z); // 이동할 위치

		PlayerCharacter->SetActorLocation(NewLocation);
	}
}

void AVRProjectGameModeBase::InitializeGameObjects()
{
	UE_LOG(LogTemp, Warning, TEXT("Call InitializeGameObjects"));
	//적 스포너가 존재하지않고, 스포너 정보가 없다면 새로 스폰
	if (BPEnemySpawner)
	{
		if (bEnemySpawnerExists == false)
		{
			if (!IsValid(EnemySpanwer))
			{
				EnemySpanwer = GetWorld()->SpawnActor<AEnemySpawner>(BPEnemySpawner, FVector(FVector::ZeroVector), FRotator(0, 0, 0));		
				UE_LOG(LogTemp, Warning, TEXT("Enemy Spawner InValid"));
			}
			
			bEnemySpawnerExists = true;
		}
		else // 스포너가 있다면, 비활성화 되있으므로, 재활성화
		{
			if (IsValid(EnemySpanwer))
			{
				UE_LOG(LogTemp, Warning, TEXT("Enemy Spawner Valid"));
				EnemySpanwer->ActivateEnemySpawner();
			}
			else
			{
				EnemySpanwer = GetWorld()->SpawnActor<AEnemySpawner>(BPEnemySpawner, FVector(FVector::ZeroVector), FRotator(0, 0, 0));
				UE_LOG(LogTemp, Warning, TEXT("Enemy Spawner InValid"));
			}
			
		} 
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Make sure to assign this in the editor before running the game."));
	}

	// 아이템 스포너가 존재하지 않고, 등록된 스포너가 없다면 아이템 스포너 스폰
	if (BPItemSpawner)
	{
		if (bItemSpawnerExists == false)
		{
			if (!IsValid(ItemSpanwer))
			{
				AItemSpawnActor* ItemSpawner = GetWorld()->SpawnActor<AItemSpawnActor>(BPItemSpawner, FVector(0, 0, 0), FRotator(0, 90.0f, 0));
				bItemSpawnerExists = true;
			}
		}	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Make sure to assign this in the editor before running the game."));
	}
}

// 플레이어가 죽었을 때 델리게이트 동작.
void AVRProjectGameModeBase::OnPlayerDeath()
{
	bPlayerAlive = false;
	if (IsValid(EnemySpanwer))
	{
		EnemySpanwer->DeActivateEnemySpawner();
	}

	return;
}

// 적이 죽었을 때 델리게이트 동작
void AVRProjectGameModeBase::OnEnemyDeath()
{
	CurrentKillCnt++;
	CheckGameClear();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, TEXT("OnEnemyDeath"));
}

void AVRProjectGameModeBase::CheckGameClear()
{
	UE_LOG(LogTemp, Warning, TEXT("call CheckGameClear"));
	if (RequiredKillCnt <= CurrentKillCnt)
	{
		TriggerGameClear();
	}
	else
	{
		return;
	}
}

/*
메모리 해제 로직
*/

void AVRProjectGameModeBase::CleanupGameItem()
{
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameItem::StaticClass(), FoundActor);

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

void AVRProjectGameModeBase::CleanupItemSpanwer()
{
	if (IsValid(ItemSpanwer))
	{
		ItemSpanwer->Destroy();
		bItemSpawnerExists = false;
	}
}

void AVRProjectGameModeBase::CleanupEnemySpawner()
{
	if (IsValid(EnemySpanwer))
	{
		EnemySpanwer->DestroyEnemy();
		EnemySpanwer->Destroy();
		bEnemySpawnerExists = false;
	}
}