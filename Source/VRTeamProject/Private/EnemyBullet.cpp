// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AEnemyBullet::AEnemyBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	SetRootComponent(BulletMesh);

	// 충돌 설정 추가 시작
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BulletMesh->SetCollisionObjectType(ECC_GameTraceChannel1); // 커스텀 채널 가능, 없으면 ECC_WorldDynamic도 사용 가능
	BulletMesh->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널 무시
	BulletMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // 플레이어(Pawn)만 Block 설정
	// 충돌 설정 추가 끝

	BulletMesh->SetNotifyRigidBodyCollision(true);
	BulletMesh->SetSimulatePhysics(false);
	BulletMesh->OnComponentHit.AddDynamic(this, &AEnemyBullet::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 4500.f;
	ProjectileMovement->MaxSpeed = 4500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 0.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

}

void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMovement)
	{
		ProjectileMovement->Activate();    // 발사체 모듈 명시적 활성화
	}

}

void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
		return;

	// 로그 출력
	UE_LOG(LogTemp, Warning, TEXT("총알이 %s (태그: %s, 채널: %d) 에 부딪힘 위치: %s"),
		*OtherActor->GetName(),
		(OtherActor->Tags.IsValidIndex(0) ? *OtherActor->Tags[0].ToString() : TEXT("없음")),
		(int32)OtherComp->GetCollisionObjectType(),
		*Hit.ImpactPoint.ToString());

	// 플레이어에게 데미지 적용
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		const float FinalDamage = Damage - Player->GetDef();
		const float NewHp = Player->GetHp() - FinalDamage;

		if (NewHp > 0)
		{
			Player->SetHp(NewHp);
			Player->NotifyPlayerChangeHealth();
		}
		else
		{
			Player->NotifyPlayerDeath();
			Player->PlayerDeSpawn();
		}
	}

	// 충돌 시 즉시 파괴
	Destroy();

	//UE_LOG(LogTemp, Warning, TEXT("총알이 %s 에 부딪힘"), *OtherActor->GetName());
	// Destroy();
	
}
