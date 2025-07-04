// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "Components/SphereComponent.h"
#include "PlayerBulletActor.h"
#include <PlayerCharacter.h>
#include "MotionControllerComponent.h"

/*
	������ �޸� �Ҵ�� ������ PlayerCharacter���� ó����
*/

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshData(TEXT("/Script/Engine.Skeleton'/Game/Asset/Weapon/rifle_001_Skeleton.rifle_001_Skeleton'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponSkeletalData(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Weapon/rifle_001.rifle_001'"));
	/// Script / Engine.Skeleton'/Game/Asset/Weapon/pistol_001_Skeleton.pistol_001_Skeleton'
	///Script/Engine.SkeletalMesh'/Game/Asset/Weapon/pistol_001.pistol_001'

	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	SetRootComponent(WeaponCollision);

	WeaponCollision->SetRelativeScale3D(FVector(1.0f,1.0f,1.0f));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletal"));
	WeaponSkeletal->SetupAttachment(WeaponCollision);
	
	if (WeaponMeshData.Succeeded())
	{
		WeaponMeshAsset = WeaponMeshData.Object;
		
	}
	if (WeaponSkeletalData.Succeeded())
	{
		WeaponSkeletal->SetSkeletalMesh(WeaponSkeletalData.Object);
		WeaponSkeletal->SetRelativeScale3D(FVector(0.4f,0.4f,0.4));
		WeaponSkeletal->SetRelativeRotation(FRotator(180.0f,180.0f,0));
		WeaponSkeletal->SetRelativeLocation(FVector(0,0,0.3f));
	}
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(WeaponMeshAsset))
	{
		WeaponMesh->SetStaticMesh(WeaponMeshAsset);
	}

	//WeaponMesh->AttachToComponent(WeaponCollision, FAttachmentTransformRules::KeepRelativeTransform);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);
}


void APlayerWeapon::Fire(float Damage)
{
	if (!bIsFire) return;

	ChangeFireState();
	//�ӽ� �ڵ�.
	//ī�޶��� ȸ������ �޾Ƽ� �Ѿ��� ������ ������.
	//VRȰ�� �� VR��Ʈ�ѷ��� ������ �޾Ƽ� ������ �ٽ� �������־�� �Ҽ�����.
	// 
	//FRotator StartRotation = GetOwner()->GetInstigatorController()->GetControlRotation();
	 
	//VR
	//const FRotator StartLeftRotation = Player->GetMotionControllerLeft()->GetRelativeRotation();
	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()->GetInstigatorController()->GetPawn());
	const FRotator StartRightRotation = Player->GetMotionControllerRight()->GetRelativeRotation();
	const FVector StartRightLocation = WeaponSkeletal->GetSocketLocation("rifle_shot");
	 
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // �׻� �������

	/////////////////////////////////////////////////////////////////////////////////////////
	APlayerBulletActor* NewBullet = GetWorld()->SpawnActor<APlayerBulletActor>(APlayerBulletActor::StaticClass(), StartRightLocation, StartRightRotation, SpawnParams);
	if (IsValid(NewBullet))
	{
		NewBullet->SetOwner(this);
		NewBullet->SetDamage(Damage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet Data invalid"));
	}
	//�Ѿ��� 0.2�ʸ��� �߻�� �� �ֵ��� Ÿ�̸Ӽ���.
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

