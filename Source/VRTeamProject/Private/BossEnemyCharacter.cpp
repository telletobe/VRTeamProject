// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemyCharacter.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBullet.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "BossEnemy_Anim.h"
#include "TimerManager.h"

ABossEnemyCharacter::ABossEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMaxHp(200.0f);  // 보스 체력
	SetCurrentHp(GetMaxHp());
	SetAtk(10.0f);
	SetDef(3.0f);
}

void ABossEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetRelativeLocation(FVector::ZeroVector);

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 1000.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		SetActorLocation(HitResult.ImpactPoint + FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	}



	UCapsuleComponent* BossCapsule = GetCapsuleComponent();
	if (BossCapsule)
	{
		BossCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BossCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		BossCapsule->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); // 플레이어 총알만 감지
		BossCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemyCharacter::OnBeginOverlap);
	}

	USkeletalMeshComponent* BossMesh = GetMesh();
	if (BossMesh)
	{
		BossMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 테스트용 강제 활성화
	bIsAttacking = true;

	// 시작하자마자 돌 던지기
	PerformThrowAttack();

	

}

void ABossEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 계속 점검되도록
	if (!IsActive() || IsDeathAnim())
		return;

	if (!bIsAttacking)
	{
		float HpPercent = GetCurrentHp() / GetMaxHp();

		bIsAttacking = true;

		if (HpPercent <= 0.8f)
			PerformSpinAttack();
		else
			PerformThrowAttack();
	}
}

void ABossEnemyCharacter::PerformThrowAttack()
{

	UE_LOG(LogTemp, Warning, TEXT("PerformThrowAttack() 호출됨"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UBossEnemy_Anim* BossAnim = Cast<UBossEnemy_Anim>(AnimInstance);
	if (BossAnim)
	{
		// 상태머신 공격 상태로 전환 (애니메이션 루프 시작)
		BossAnim->bIsThrowing = true;
		UE_LOG(LogTemp, Warning, TEXT("애님 상태 변경: IsThrowing = true"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cast<UBossEnemy_Anim> 실패"));
	}

		//// 이미 타이머가 활성화되어 있다면 중복 방지
		//if (!GetWorldTimerManager().IsTimerActive(LaunchRockHandle))
		//{
		//	GetWorldTimerManager().SetTimer(LaunchRockHandle, this, &ABossEnemyCharacter::LaunchRock, 1.5f, true);
		//}
	
}

void ABossEnemyCharacter::PerformSpinAttack()
{
	if (SpinMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(SpinMontage);

			// 애니메이션 끝나면 공격 상태 초기화
			float Duration = SpinMontage->GetPlayLength();
			GetWorldTimerManager().SetTimer(AttackCooldownHandle, [this]()
				{
					bIsAttacking = false;
				}, Duration, false);
		}
	}
	else
	{
		bIsAttacking = false;
	}
}

void ABossEnemyCharacter::LaunchRock()
{

UE_LOG(LogTemp, Warning, TEXT("보스가 돌을 던졌습니다! (LaunchRock 호출됨)"));

	if (!EnemyBulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyBulletClass가 설정되지 않았습니다."));
		return;
	}

	// 보스 오른손 소켓 위치
	FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("RightHand"));

	// 플레이어 캐릭터 가져오기 (인덱스 0)
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어를 찾을 수 없습니다."));
		return;
	}

	// 플레이어 방향 계산 및 정규화
	FVector DirectionToPlayer = (Player->GetActorLocation() - SpawnLocation).GetSafeNormal();

	// 발사체 회전은 방향 벡터 기준
	FRotator DirectionRotation = DirectionToPlayer.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	AEnemyBullet* Bullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulletClass, SpawnLocation, DirectionRotation, SpawnParams);

	if (Bullet && Bullet->ProjectileMovement)
	{
		// 보스 자신과 충돌하지 않도록 설정
		Bullet->BulletMesh->IgnoreActorWhenMoving(this, true);

		// 보스와 동일한 ObjectType(Pawn)에 대해 충돌 무시
		Bullet->BulletMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		// ProjectileMovement 활성화 및 속도 설정
		// SetUpdatedComponent 호출은 생성자에서 이미 처리했으므로 주석 처리 또는 제거
		// Bullet->ProjectileMovement->SetUpdatedComponent(Bullet->BulletMesh);

		// 속도 설정 (InitialSpeed 기준)
		Bullet->ProjectileMovement->Velocity = DirectionToPlayer * Bullet->ProjectileMovement->InitialSpeed;
		Bullet->ProjectileMovement->Activate(true);

		UE_LOG(LogTemp, Warning, TEXT("돌멩이 발사 방향: %s"), *DirectionToPlayer.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet 스폰 실패 또는 ProjectileMovement 없음"));
	}


}

void ABossEnemyCharacter::SetIsThrowing(bool bThrowing)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (UBossEnemy_Anim* BossAnim = Cast<UBossEnemy_Anim>(AnimInstance))
	{
		BossAnim->bIsThrowing = bThrowing;
	}


}

void ABossEnemyCharacter::NotifyThrowProjectile()
{

	LaunchRock();

}
