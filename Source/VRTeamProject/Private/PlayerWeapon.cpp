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

	//�ӽ� �ڵ�.
	//ī�޶��� ȸ������ �޾Ƽ� �Ѿ��� ������ ������.
	//VRȰ�� �� VR��Ʈ�ѷ��� ������ �޾Ƽ� ������ �ٽ� �������־�� �Ҽ�����.
	FRotator StartRotation = GetOwner()->GetInstigatorController()->GetControlRotation();

	/////////////////////////////////////////////////////////////////////////////////////////
	APlayerBulletActor* NewBullet = GetWorld()->SpawnActor<APlayerBulletActor>(APlayerBulletActor::StaticClass(),GetActorLocation(), StartRotation);
	NewBullet->SetOwner(this);
	NewBullet->SetDamage(Damage);

	//�Ѿ��� 0.2�ʸ��� �߻�� �� �ֵ��� Ÿ�̸Ӽ���.
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

