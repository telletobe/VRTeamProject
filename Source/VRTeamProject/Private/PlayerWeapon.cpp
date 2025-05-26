// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "Components/SphereComponent.h"
#include "PlayerBulletActor.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshData(TEXT("'/Engine/VREditor/TransformGizmo/SM_Sequencer_Key.SM_Sequencer_Key'"));

	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	SetRootComponent(WeaponCollision);

	WeaponCollision->SetRelativeScale3D(FVector(1.0f,1.0f,1.0f));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	if (WeaponMeshData.Succeeded())
	{
		WeaponMesh->SetStaticMesh(WeaponMeshData.Object);
	}

	WeaponMesh->AttachToComponent(WeaponCollision, FAttachmentTransformRules::KeepRelativeTransform);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
}

void APlayerWeapon::Fire(float Damage)
{
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Black,TEXT("APlayerWeapon : Fire()"));

	if (!bIsFire) return;

	bIsFire = false;
	APlayerBulletActor* NewBullet = GetWorld()->SpawnActor<APlayerBulletActor>(APlayerBulletActor::StaticClass(),GetActorLocation(), FRotator(0));

	GetWorld()->GetTimerManager().SetTimer(FireTimer,this,&APlayerWeapon::ChangeFireState,0.2f,false);

}

void APlayerWeapon::ChangeFireState()
{
	bIsFire = true;
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

