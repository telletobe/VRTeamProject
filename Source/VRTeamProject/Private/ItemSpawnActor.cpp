// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnActor.h"
#include <GameItem.h>
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/TargetPoint.h"
#include "VRProjectGameModeBase.h"
#include "Sound/SoundCue.h"


// Sets default values
AItemSpawnActor::AItemSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> AirPlaneSoundData(TEXT("/Script/Engine.SoundCue'/Game/Audio/EffectSound/Airplane_Cue.Airplane_Cue'"));

	ItemSpawnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemSpawnerCollision"));

	SetRootComponent(ItemSpawnerCollision);

	ItemSpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemSpawnerMesh"));
	ItemSpawnerMesh->AttachToComponent(ItemSpawnerCollision, FAttachmentTransformRules::KeepRelativeTransform);

	SpawnDelay = 3.0f;
	DropDelay = 2.5f;

	if (AirPlaneSoundData.Succeeded())
	{
		AirPlaneSound = AirPlaneSoundData.Object;
	}
}

void AItemSpawnActor::SpawnItem()
{
	if (GameMode->IsPlayerAlive())
	{
		const FVector SpawnPoint = FMath::RandPointInBox(ItemSpawnerCollision->Bounds.GetBox());

		const int32 ItemDropTableCnt = 4; 

		
		if (GameItemClass)
		{
			AGameItem* SpawnedItem = GetWorld()->SpawnActor<AGameItem>(GameItemClass, SpawnPoint, FRotator(0));
			const int32 ItemType = FMath::RandRange(0, ItemDropTableCnt - 1); //������ ����� ����. Ȯ���� ����.

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
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Please assign item value in the editor. "));
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
	if (!GameMode->IsClear()) 
	{
		if (!bIsActive)
		{
			bIsActive = true;
			SetActorHiddenInGame(false);
			PrimaryActorTick.bCanEverTick = true;
			GetWorld()->GetTimerManager().ClearTimer(ActorVisibleHandle);
			SetDropTimer();
			if (AirPlaneSound)
			{
				UGameplayStatics::PlaySound2D(this, AirPlaneSound);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AirPlaneSound Data invalid"));
			}
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