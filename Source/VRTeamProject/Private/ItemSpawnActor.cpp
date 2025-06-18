// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnActor.h"
#include <GameItem.h>
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/TargetPoint.h"
#include "VRProjectGameModeBase.h"
// Sets default values
AItemSpawnActor::AItemSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemSpawnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemSpawnerCollision"));

	SetRootComponent(ItemSpawnerCollision);

	ItemSpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemSpawnerMesh"));
	ItemSpawnerMesh->AttachToComponent(ItemSpawnerCollision, FAttachmentTransformRules::KeepRelativeTransform);

}

void AItemSpawnActor::SpawnItem()
{
	FVector SpawnPoint = FMath::RandPointInBox(ItemSpawnerCollision->Bounds.GetBox());

	TSubclassOf<AGameItem> Item = LoadClass<AGameItem>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Actor/Item/MyGameItem.MyGameItem_C'"));
	if (Item)
	{
		AGameItem* SpawnedItem = GetWorld()->SpawnActor<AGameItem>(Item, SpawnPoint, FRotator(0));
		//SpawnedItem->SetItemData(EItemEffectData::AttackSpeed);
		return;
	}

}

void AItemSpawnActor::MoveToEndPoint(float DeltaTime)
{
	FVector NewLocation = GetActorLocation() + MoveForce * DeltaTime;
	SetActorLocation(NewLocation);
	ResetLocationToStartPoint();
}

void AItemSpawnActor::FindTartgetPoint()
{
	TArray<AActor*> TargetPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), TargetPoint);
	for (auto ItemPoint : TargetPoint)
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
		/*	else if (tempPoint->ActorHasTag("DropPoint"))
			{
				DropPoint = tempPoint;
			}*/
		}
	}
}

void AItemSpawnActor::ChangeActiveState()
{
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
	if (bIsActive)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnItemHandle, this, &AItemSpawnActor::SpawnItem, DropDelay, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnItemHandle);
	}
}

void AItemSpawnActor::ResetLocationToStartPoint()
{

	if (IsValid(EndPoint))
	{
		if (IsValid(StartPoint))
		{
			float EndDistance = FVector::Dist2D(GetActorLocation(), EndPoint->GetActorLocation());
			if (EndDistance < 100.0f)
			{
				SetActorLocation(StartPoint->GetActorLocation());
				ChangeActiveState();
				return;
			}

		}
	}

}

//void AItemSpawnActor::CreateItemSpawnActor()
//{
//	if (BPItemSpawner)
//	{
//		AItemSpawnActor* ItemSpawner = GetWorld()->SpawnActor<AItemSpawnActor>(BPItemSpawner, FVector(0, 0, 0),FRotator(0,90.0f,0));
//	}
//}

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

