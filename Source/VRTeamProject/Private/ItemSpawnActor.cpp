// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnActor.h"
#include <GameItem.h>
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/TargetPoint.h"
#include "VRProjectGameModeBase.h"

/*
	아이템 스포너 -> 비행기로 표시
	메모리 해제는 게임모드에서 관리
*/

// Sets default values
AItemSpawnActor::AItemSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemSpawnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemSpawnerCollision"));

	SetRootComponent(ItemSpawnerCollision);

	ItemSpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemSpawnerMesh"));
	ItemSpawnerMesh->AttachToComponent(ItemSpawnerCollision, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnDelay = 3.0f;
	DropDelay = 2.5f;
}

void AItemSpawnActor::SpawnItem()
{
	//게임모드에서 플레이어의 상태를 받아와서 비행기가 나타 날 때, 플레이어가 살아 있다면 아이템을 드랍.
	if (GameMode->IsPlayerAlive())
	{
		const FVector SpawnPoint = FMath::RandPointInBox(ItemSpawnerCollision->Bounds.GetBox());

		const int32 ItemDropTableCnt = 4; //ItemDropTable값은 ItemTypeEnum 변경시 수치변경 필요

		TSubclassOf<AGameItem> Item = LoadClass<AGameItem>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Actor/Item/MyGameItem.MyGameItem_C'"));
		if (Item)
		{
			AGameItem* SpawnedItem = GetWorld()->SpawnActor<AGameItem>(Item, SpawnPoint, FRotator(0));
			const int32 ItemType = FMath::RandRange(0, ItemDropTableCnt - 1); //아이템 드랍은 랜덤. 확률은 없음.

			switch (ItemType)
			{
			case 0:
				SpawnedItem->SetItemData(EItemEffectData::HEAL);
				break;
			case 1:
				SpawnedItem->SetItemData(EItemEffectData::AtkUp);
				break;
			case 2:
				SpawnedItem->SetItemData(EItemEffectData::DefUp);
				break;
			case 3:
				SpawnedItem->SetItemData(EItemEffectData::AttackSpeed);
				break;

			default:
				return;
				break;
			}
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("ItemSpawnActor(SpawnItem()) : Player IS Dead"));
		return;
	}
}

void AItemSpawnActor::ChangeActiveState()
{
	// beginPlay에서 바인딩 걸어 둔 함수 
	// 현재 게임클리어 상태에 따라서 아이템을 드랍해주는 비행기를 나타나게 하거나, 나타나지 않게 함.
	if (!GameMode->IsClear()) 
	{
		if (!bIsActive)
		{
			bIsActive = true;
			SetActorHiddenInGame(false);
			PrimaryActorTick.bCanEverTick = true;
			GetWorld()->GetTimerManager().ClearTimer(ActorVisibleHandle);
			SetDropTimer();
		}
		else
		{
			bIsActive = false;
			SetActorHiddenInGame(true);
			PrimaryActorTick.bCanEverTick = false;
			GetWorld()->GetTimerManager().SetTimer(ActorVisibleHandle, this, &AItemSpawnActor::ChangeActiveState, SpawnDelay, true);
			SetDropTimer();
		}
	}
	else
	{
		bIsActive = false;
		SetActorHiddenInGame(true);
		PrimaryActorTick.bCanEverTick = false;
		GetWorldTimerManager().ClearTimer(ActorVisibleHandle);
	}
}

void AItemSpawnActor::SetDropTimer()
{
	//비행기는 일정시간마다 아이템을 드랍하며, 비행기가 활성화되있지않으면 아이템을 드랍하지않음.
	if (bIsActive)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnItemHandle, this, &AItemSpawnActor::SpawnItem, DropDelay, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnItemHandle);
	}
}

// Called when the game starts or when spawned
void AItemSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	if (GameMode == nullptr)
	{
		GameMode = Cast<AVRProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	}
	ItemSpawnerCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ItemSpawnerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemSpawnerCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MoveForce = FVector(-1200, 0, 0);

	SetActorHiddenInGame(true);
	PrimaryActorTick.bCanEverTick = false;
	FindTartgetPoint();

	if (!bIsActive)
	{
		GetWorld()->GetTimerManager().SetTimer(ActorVisibleHandle, this, &AItemSpawnActor::ChangeActiveState, SpawnDelay, true);
	}
}

// Called every frame
void AItemSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		MoveToEndPoint(DeltaTime);
	}
}

//아래의 함수들은 비행기의 이동을 담당하는 함수

void AItemSpawnActor::MoveToEndPoint(float DeltaTime)
{
	const FVector NewLocation = GetActorLocation() + MoveForce * DeltaTime;
	SetActorLocation(NewLocation);
	ResetLocationToStartPoint();
}

void AItemSpawnActor::FindTartgetPoint()
{
	TArray<AActor*> TargetPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetPoint);
	for (const auto& ItemPoint : TargetPoint)
	{
		if (ATargetPoint* tempPoint = Cast<ATargetPoint>(ItemPoint))
		{
			if (tempPoint->ActorHasTag("StartPoint"))
			{
				StartPoint = tempPoint;
				SetActorLocation(StartPoint->GetActorLocation());
			}
			else if (tempPoint->ActorHasTag("EndPoint"))
			{
				EndPoint = tempPoint;
			}
		}
	}
}

void AItemSpawnActor::ResetLocationToStartPoint()
{
	if (IsValid(EndPoint))
	{
		if (IsValid(StartPoint))
		{
			const float EndDistance = FVector::Dist2D(GetActorLocation(), EndPoint->GetActorLocation());
			if (EndDistance < 100.0f)
			{
				SetActorLocation(StartPoint->GetActorLocation());
				ChangeActiveState();
				return;
			}
		}
	}
}

//////이동 함수 끝