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
		WeaponMeshAsset = WeaponMeshData.Object;
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

	WeaponMesh->AttachToComponent(WeaponCollision, FAttachmentTransformRules::KeepRelativeTransform);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetSimulatePhysics(false);

}


void APlayerWeapon::Fire(float Damage)
{
	if (!bIsFire) return;

	ChangeFireState();

	//임시 코드.
	//카메라의 회전값을 받아서 총알의 방향을 정해줌.
	//VR활용 시 VR컨트롤러의 정보를 받아서 방향을 다시 설정해주어야 할수있음.
	FRotator StartRotation = GetOwner()->GetInstigatorController()->GetControlRotation();

	/////////////////////////////////////////////////////////////////////////////////////////
	APlayerBulletActor* NewBullet = GetWorld()->SpawnActor<APlayerBulletActor>(APlayerBulletActor::StaticClass(),GetActorLocation(), StartRotation);
	NewBullet->SetOwner(this);
	NewBullet->SetDamage(Damage);

	//총알이 0.2초마다 발사될 수 있도록 타이머설정.
	FTimerHandle FireTimer;
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

