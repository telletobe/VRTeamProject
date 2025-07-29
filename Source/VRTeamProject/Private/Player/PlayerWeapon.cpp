// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerWeapon.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerBulletActor.h"
#include "Player/PlayerCharacter.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponSkeletalData(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Weapon/rifle_001.rifle_001'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> FireSoundData(TEXT("/Script/Engine.SoundCue'/Game/Audio/EffectSound/gunshot_Cue.gunshot_Cue'"));
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
		WeaponSkeletal->SetRelativeScale3D(FVector(9.0f, 9.0f, 9.0f));
		WeaponSkeletal->SetRelativeRotation(FRotator(180.0f,-180.0f,-180.0f));
		WeaponSkeletal->SetRelativeLocation(FVector(0,0,-15.0f));
	}

	if (FireSoundData.Succeeded())
	{
		FireSound = FireSoundData.Object;
	}
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponSkeletal->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponSkeletal->SetSimulatePhysics(false);

}

FRotator APlayerWeapon::FireWithSpread(float Pitch,float Yaw,float Roll)
{
	const float RandomPitch = FMath::FRandRange(-Pitch, Pitch); // Y
	const float RandomYaw = FMath::FRandRange(-Yaw, Yaw); // Z
	const float RandomRoll = FMath::FRandRange(-Roll, Roll); // X
	const FQuat RandomQuat = FRotator(RandomPitch, RandomYaw, RandomRoll).Quaternion();

	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()->GetInstigatorController()->GetPawn());
	if (Player)
	{
		const FQuat BaseQuat = Player->GetMotionControllerRight()->GetRelativeRotation().Quaternion();
		const FQuat FinalQuat = RandomQuat * BaseQuat;
		const FRotator FinalRotator = FinalQuat.Rotator();

		return FinalRotator;
	}
	else
	{
		return FRotator::ZeroRotator;
	}
}


void APlayerWeapon::Fire(const float Damage)
{
	if (!bIsFire) return;

	ChangeFireState();
	
	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()->GetInstigatorController()->GetPawn());
	const FVector StartRightLocation = WeaponSkeletal->GetSocketLocation("rifle_shot");
	const FRotator SpreadAngle = FireWithSpread(0.0f, 2.0f, 2.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APlayerBulletActor* NewBullet = GetWorld()->SpawnActor<APlayerBulletActor>(APlayerBulletActor::StaticClass(), FVector::ZeroVector, SpreadAngle, SpawnParams);

//	APlayerBulletActor* NewBullet = GetWorld()->SpawnActor<APlayerBulletActor>(APlayerBulletActor::StaticClass(), StartRightLocation, SpreadAngle, SpawnParams);
	if (FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, FireSound,GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FireSound Data invalid"));
	}

	if (IsValid(NewBullet))
	{
		NewBullet->SetActorLocation(StartRightLocation);
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

void APlayerWeapon::SetFireDelay(const float AttackFireDelay)
{
	FireDelay = AttackFireDelay;
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

