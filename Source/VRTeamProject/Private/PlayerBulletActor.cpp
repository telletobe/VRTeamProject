// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBulletActor.h"
#include "Components/SphereComponent.h"

// Sets default values
APlayerBulletActor::APlayerBulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMeshData(TEXT("'/Engine/EditorMeshes/EditorSphere.EditorSphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BulletMaterialData(TEXT("'/Engine/MapTemplates/Materials/BasicAsset03.BasicAsset03'"));

	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollision"));
	SetRootComponent(BulletCollision);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->AttachToComponent(BulletCollision, FAttachmentTransformRules::KeepRelativeTransform);

	if (BulletMeshData.Succeeded())
	{
		BulletMesh->SetStaticMesh(BulletMeshData.Object);
	}

	if (BulletMaterialData.Succeeded())
	{
		BulletMesh->SetMaterial(0,BulletMaterialData.Object);
	}

	BulletCollision->SetRelativeScale3D(FVector(0.25f));
	BulletMesh->SetRelativeScale3D(FVector(0.25f));


	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


void APlayerBulletActor::BulletMove()
{
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * MoveInterval;
	SetActorLocation(NewLocation);
}




// Called when the game starts or when spawned
void APlayerBulletActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle MoveTimerHandle;
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle,this,&APlayerBulletActor::BulletMove, MoveInterval,true);

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateLambda([this]() {
		Destroy();
		}),
		2.0F,false);
}

// Called every frame
void APlayerBulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



