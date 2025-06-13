// Fill out your copyright notice in the Description page of Project Settings.


#include "GameItem.h"
#include "Components/BoxComponent.h"
#include <PlayerCharacter.h>
#include "PlayerBulletActor.h"
#include "Engine/StaticMeshActor.h"


// Sets default values
AGameItem::AGameItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollision"));

	SetRootComponent(ItemCollision);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->AttachToComponent(ItemCollision, FAttachmentTransformRules::KeepRelativeTransform);

	ParachuteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ParachuteMesh"));
	ParachuteMesh->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);

	BoxTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxTopMesh"));
	BoxTopMesh->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void AGameItem::SetHp(float ItemHp)
{
	if (ItemHp < 0)
	{
		Hp = MaxHp;
	} else
	{
		Hp = ItemHp;
	}
}

void AGameItem::SetItemData(EItemEffectData EItemData)
{
	ItemData = EItemData;
}

// Called when the game starts or when spawned
void AGameItem::BeginPlay()
{
	Super::BeginPlay();

	ItemCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemCollision->SetCollisionObjectType(ECC_WorldDynamic);
	ItemCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetMassOverrideInKg(NAME_None, 10000.0f, true);
	ItemMesh->SetNotifyRigidBodyCollision(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ItemMesh->SetLinearDamping(5.0f);

	ItemData = EItemEffectData::HEAL;

	if (ItemMesh)
	{
		ItemMesh->SetMassOverrideInKg(NAME_None, 10000.0f, true);
		ItemMesh->OnComponentHit.AddDynamic(this,&AGameItem::OnHit);
	}
}

//이벤트 발생자(EventInstigator)는 null값이 들어오니 사용하면 안됨.
float AGameItem::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if ((Player = Cast<APlayerCharacter>(DamageCauser->GetOwner())))
	{
		UE_LOG(LogTemp,Warning,TEXT("GameItem.cpp : TakeDamage Get Player Info!"));
	}

	return 0.0f;
}

void AGameItem::Destroyed()
{
	Super::Destroyed();

	if (IsValid(Player))
	{
		Player->ApplyEffectItem(ItemData);
	}
}

void AGameItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("OnGroundHit"));

	if (OtherActor)
	{
		ParachuteMesh->SetVisibility(false);
		ItemMesh->OnComponentHit.RemoveDynamic(this,&AGameItem::OnHit);
	}

	return;
}


// Called every frame
void AGameItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

