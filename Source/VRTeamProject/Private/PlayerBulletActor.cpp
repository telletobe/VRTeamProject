// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBulletActor.h"
#include "Components/SphereComponent.h"
#include <GameItem.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
APlayerBulletActor::APlayerBulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Asset Data Load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMeshData(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/MaterialSphere.MaterialSphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BulletMaterialData(TEXT("'/Engine/MapTemplates/Materials/BasicAsset03.BasicAsset03'"));

	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollision"));
	SetRootComponent(BulletCollision);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->AttachToComponent(BulletCollision, FAttachmentTransformRules::KeepRelativeTransform);

	if (BulletMeshData.Succeeded())
	{
		BulletMeshAsset = BulletMeshData.Object;
	}

	if (BulletMaterialData.Succeeded())
	{
		BulletMeshMaterial = BulletMaterialData.Object;
	}

	BulletCollision->SetRelativeScale3D(FVector(0.25f));
	BulletMesh->SetRelativeScale3D(FVector(0.25f));
}

// Called when the game starts or when spawned
void APlayerBulletActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BulletMeshAsset))
	{
		BulletMesh->SetStaticMesh(BulletMeshAsset);
	}

	if (IsValid(BulletMeshMaterial))
	{
		BulletMesh->SetMaterial(0, BulletMeshMaterial);
	}

	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (IsValid(BulletCollision))
	{
		BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerBulletActor::OnBeginOverlap);
	}

	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &APlayerBulletActor::BulletMove, MoveInterval, true);
	SetLifeSpan(2.0f);
}

void APlayerBulletActor::BulletMove()
{
	const FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * MoveInterval;
	SetActorLocation(NewLocation);
}

void APlayerBulletActor::SetDamage(float BulletDamage)
{
	Damage = BulletDamage;
}

void APlayerBulletActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	// 아이템을 부수기위한 이벤트처리
	if (AGameItem* Item = Cast<AGameItem>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(Item, GetDamage(), nullptr, GetOwner(), nullptr); // 총알의 데미지와 총알의오너(무기) 정보 넘겨줌

		if (Item->GetHp() > 0)
		{
			float ItemHp = Item->GetHp() - GetDamage();
			if (ItemHp > 0)
			{
				Item->SetHp(ItemHp);
			}
			else
			{
				Item->Destroy();
			}
			Destroy();
		}
	}
}

// Called every frame
void APlayerBulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
