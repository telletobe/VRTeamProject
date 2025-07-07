// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "Components/SphereComponent.h"
#include "PlayerBulletActor.h"
#include <PlayerCharacter.h>
#include "MotionControllerComponent.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponSkeletalData(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Weapon/rifle_001.rifle_001'"));
	///Script/Engine.SkeletalMesh'/Game/Asset/Weapon/pistol_001.pistol_001'

	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	SetRootComponent(WeaponCollision);

	WeaponCollision->SetRelativeScale3D(FVector(1.0f,1.0f,1.0f));

	WeaponSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletal"));
	WeaponSkeletal->SetupAttachment(WeaponCollision);
	WeaponSkeletal->SetGenerateOverlapEvents(false);
	
	if (WeaponSkeletalData.Succeeded())
	{
		WeaponSkeletal->SetSkeletalMesh(WeaponSkeletalData.Object);
		WeaponSkeletal->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
		WeaponSkeletal->SetRelativeRotation(FRotator(180.0f,180.0f,0));
		WeaponSkeletal->SetRelativeLocation(FVector(0,0,0.3f));
	}
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponSkeletal->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletal->SetSimulatePhysics(false);
}


void APlayerWeapon::Fire(float Damage)
{
	if (!bIsFire) return;

	ChangeFireState();
	//FRotator StartRotation = GetOwner()->GetInstigatorController()->GetControlRotation();
	 
	//VR
	//const FRotator StartLeftRotation = Player->GetMotionControllerLeft()->GetRelativeRotation();
	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()->GetInstigatorController()->GetPawn());
	const FRotator StartRightRotation = Player->GetMotionControllerRight()->GetRelativeRotation();
	const FVector StartRightLocation = WeaponSkeletal->GetSocketLocation("rifle_shot");
	 
	//
	// »êÅºÈ¿°ú
	const FQuat BaseQuat = Player->GetMotionControllerRight()->GetRelativeRotation().Quaternion();
	const float SpreadAngle = 5.0f;
	//const float RandomPitch = FMath::FRandRange(-SpreadAngle, SpreadAngle); // Y
	const float RandomYaw = FMath::FRandRange(-SpreadAngle, SpreadAngle); // Z
	const float RandomRoll = FMath::FRandRange(-SpreadAngle, SpreadAngle); // X
	const FQuat RandomQuat = FRotator(0.0f, RandomYaw, RandomRoll).Quaternion();
	const FQuat FinalQuat = RandomQuat * BaseQuat;
	const FRotator FinalRotator = FinalQuat.Rotator();
	//

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	/////////////////////////////////////////////////////////////////////////////////////////
	APlayerBulletActor* NewBullet = GetWorld()->SpawnActor<APlayerBulletActor>(APlayerBulletActor::StaticClass(), StartRightLocation, FinalRotator, SpawnParams);
	if (IsValid(NewBullet))
	{
		NewBullet->SetOwner(this);
		NewBullet->SetDamage(Damage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet Data invalid"));
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &APlayerWeapon::ChangeFireState, FireDelay, false);
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(FireTimer,this,&APlayerWeapon::ChangeFireState, FireDelay,false);
}

void APlayerWeapon::ChangeFireState()
{
	bIsFire = !bIsFire;
}

void APlayerWeapon::SetFireDelay(float AttackFireDelay)
{
	FireDelay = AttackFireDelay;
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

