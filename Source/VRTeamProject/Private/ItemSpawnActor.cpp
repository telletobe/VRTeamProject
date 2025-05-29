// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnActor.h"
#include <GameItem.h>
#include "Components/BoxComponent.h"

// Sets default values
AItemSpawnActor::AItemSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemSpawnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemSpawnerCollision"));

	SetRootComponent(ItemSpawnerCollision);

	ItemSpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemSpawnerMesh"));
	ItemSpawnerMesh->AttachToComponent(ItemSpawnerCollision, FAttachmentTransformRules::KeepRelativeTransform);

	ItemSpawnerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AItemSpawnActor::SpawnItem()
{
	FVector SpawnPoint = FMath::RandPointInBox(ItemSpawnerCollision->Bounds.GetBox());
	////////////////////////////////////////////////////////////////////////////////
	// 임시 코드

	bool bIsSpawn = true;

	if (bIsSpawn)
	{
		TSubclassOf<AGameItem> Item = LoadClass<AGameItem>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Actor/Item/MyGameItem.MyGameItem_C'"));
		if (Item)
		{
			AGameItem* SpawnedItem = GetWorld()->SpawnActor<AGameItem>(Item, SpawnPoint, FRotator(0));
			if (SpawnedItem)
			{

			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("InValid Item!"));
		}
	}
	//////////////////////////////////////////////////////////////////////////////////

}

// Called when the game starts or when spawned
void AItemSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this,  &AItemSpawnActor::SpawnItem, SpawnDelay, true);
	
}

// Called every frame
void AItemSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

