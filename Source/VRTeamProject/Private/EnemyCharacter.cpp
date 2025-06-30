// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NavigationInvokerComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include <EnemyAIController.h>
#include "PlayerCharacter.h"
#include "PlayerBulletActor.h"

AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//기본 값 세팅
	SetCurrentHp(10);
	SetDef(1.0f);
	SetAtk(5.0f);
	SetSpawnDelay(5.0f);

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
	if (CharacterCollision)
	{
		CharacterCollision->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnComponentHit);
		CharacterCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnBeginOverlap);
		OnEnemyDeathAnimEnded.AddDynamic(this, &AEnemyCharacter::EnemyDeathAnimEnded);
	}


	// NavInvoker 관련 생성,삭제 변수값 설정
	NavGenerationRadius = 2000.0f;
	NavRemovalRadius = 500.0f;
	NavInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);

	UCapsuleComponent* EnemyCollision = GetCapsuleComponent();
	EnemyCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	USkeletalMeshComponent* EnemyMesh = GetMesh();
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FindSpawnPoint();
	FindDeSpawnPoint();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::SetCurrentHp(float EnemyHp)
{
	if (EnemyHp < 0)
	{
		CurrentHp = GetMaxHp();
	}
	else
	{
		CurrentHp = EnemyHp;
	}
}

void AEnemyCharacter::SetMaxHp(float EnemyHp)
{
	if (EnemyHp < 0)
	{
		return;
	}
	else
	{
		MaxHp = EnemyHp;
	}
}

void AEnemyCharacter::SetDef(float EnemyDef)
{
	if (EnemyDef < 0)
	{
		Def = DefaultDef;
	}
	else
	{
		Def = EnemyDef;
	}
}

void AEnemyCharacter::SetAtk(float EnemyAtk)
{
	if (EnemyAtk < 0)
	{

	}
	else
	{
		Atk = EnemyAtk;
	}
}

void AEnemyCharacter::FindSpawnPoint()
{
	TArray<AActor*> FoundEndPoint;
	TArray<AActor*> RandomPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundEndPoint);

	for (const auto& StartPoint : FoundEndPoint)
	{
		if (Cast<ATargetPoint>(StartPoint))
		{
			if (!SpawnPoint)
			{
				if (StartPoint->ActorHasTag(TEXT("EnemySpawnPoint")))
				{
					RandomPoint.Add(StartPoint);
				}
			}
		}
	}

	SpawnPoint = RandomPoint[FMath::RandRange(0,RandomPoint.Num()-1)];
}

void AEnemyCharacter::FindDeSpawnPoint()
{
	TArray<AActor*> FoundEndPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundEndPoint);

	for (const auto& EndPoint : FoundEndPoint)
	{
		if (Cast<ATargetPoint>(EndPoint))
		{
			if (!DeSpawnPoint)
			{
				if (EndPoint->ActorHasTag(TEXT("EnemyDeSpawnPoint")))
				{
					DeSpawnPoint = EndPoint;
				}
			}
		}
	}
}

void AEnemyCharacter::DeSpawn()
{
	if (IsValid(DeSpawnPoint))
	{
		SetActorLocation(DeSpawnPoint->GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("DeSpawnPoint invalid"));
		SetActorLocation(FVector(FVector::ZeroVector));
	}

	SetActorHiddenInGame(true);
	bIsActive = false;
	NotifyEnemyDespawn();
	SetActorTickEnabled(false);
}

void AEnemyCharacter::Spawn()
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
	APlayerCharacter* Player = Cast<APlayerCharacter>(EnemyController->GetTargetActor());
	if (Player)
	{
		EnemyController->StartChasing(Player);
	}

	bIsActive = true;
	bIsDeathAnim = false;
	bIsAttacking = false;

	if (IsValid(SpawnPoint))
	{
		SetCurrentHp(GetMaxHp());
		SetActorLocation(SpawnPoint->GetActorLocation());
	}
	else {
		UE_LOG(LogTemp,Warning,TEXT("SpawnPoint InValid "));
		FindSpawnPoint();
		Spawn();
	}
}

void AEnemyCharacter::NotifyEnemyDespawn()
{
	OnEnemyDespawned.Broadcast(); // 게임 클리어체크용
}

void AEnemyCharacter::NotifyEnemyDeath()
{
	OnEnemyDeath.Broadcast(); // 킬 카운트 증가용
}

void AEnemyCharacter::EnemyDeathAnimEnded()
{
	bIsDeathAnim = false;
	NotifyEnemyDeath();
	DeSpawn();
}

void AEnemyCharacter::PlayDeathMontage()
{
	if (const USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			if (DeathMontage)
			{
				SetActorEnableCollision(false);
				AnimInstance->Montage_Play(DeathMontage);
				bIsDeathAnim = true;
				AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
				EnemyController->StopMovement();
				
			}
		}
	}
}

void AEnemyCharacter::PlayHitMontage()
{
	if (const USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			if (HitMontage && !AnimInstance->Montage_IsPlaying(HitMontage))
			{
				bIsHitReacting = true;
				AnimInstance->Montage_Play(HitMontage);
				
				return;
			}
		}
	}
}

void AEnemyCharacter::PlayAttackMontage()
{
	if (const USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			if (AttackMontage && !AnimInstance->Montage_IsPlaying(AttackMontage))
			{
				bIsAttacking = true;
				AnimInstance->Montage_Play(AttackMontage);
				
				GetWorldTimerManager().SetTimer(AttackAnimTimerHandle, [this]()
					{
						bIsAttacking = false;
					}, AttackMontage->GetPlayLength(), false);
			}
		}
	}
}

void AEnemyCharacter::SetSpawnDelay(float EnemySpawnDelay)
{
	SpawnDelay = EnemySpawnDelay;
}

void AEnemyCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerBulletActor* Bullet = Cast<APlayerBulletActor>(OtherActor);

	if (IsValid(Bullet))
	{
		if (GetCurrentHp() > 0)
		{
			PlayHitMontage(); // 피격 반응 추가
			float EnemyHp = GetCurrentHp() - (Bullet->GetDamage() - GetDef());
			if (EnemyHp > 0)
			{
				SetCurrentHp(EnemyHp);
			}
			else
			{
				if (IsDeathAnim() == false)
				{
					FTimerHandle AnimEndNotifyHandle;
					PlayDeathMontage();
					GetWorldTimerManager().SetTimer(AnimEndNotifyHandle, [this]() {OnEnemyDeathAnimEnded.Broadcast(); }, 3.0f, false);
				}
			}
		}
		Bullet->Destroy();
	}
}

void AEnemyCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (!IsValid(Player) || bIsDeathAnim || bIsAttacking)	return;

	// 1. 적 이동 멈추기
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
	EnemyController->StopMovement();

	// 2. 공격 애니메이션 재생
	PlayAttackMontage();
	bIsAttacking = true;

	// 3. 애니메이션 끝날 때 데미지 처리 → 타이머 사용
	if (AttackMontage)
	{
		float Duration = AttackMontage->GetPlayLength(); // 애니메이션 길이
	
		GetWorldTimerManager().SetTimer(AttackAnimTimerHandle, [this, Player]()
			{
				if (!IsValid(Player)) return;

				float PlayerCurrentHp = Player->GetHp();
				float PlayerHp = PlayerCurrentHp - (GetAtk() - Player->GetDef());

				if (PlayerHp > 0)
				{
					Player->SetHp(PlayerHp);
					Player->NotifyPlayerChangeHealth();
				}
				else
				{
					Player->PlayerDeSpawn();
					Player->NotifyPlayerDeath();
					Player->ShowEndGame();
				}

				// 4. 적 비활성화
				this->DeSpawn();

			}, Duration, false);
	}
	//APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	//if (!bIsDeathAnim)
	//{
	//	PlayAttackMontage();
	//	// 데미지 처리 로직
	//}

	//if (IsValid(Player))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Player IsValid"));
	//	float PlayerCurrentHp = Player->GetHp();
	//	float PlayerHp = PlayerCurrentHp - (GetAtk() - Player->GetDef());
	//	if (PlayerHp > 0)
	//	{
	//		Player->SetHp(PlayerHp);
	//		Player->NotifyPlayerChangeHealth();
	//		
	//	}
	//	else
	//	{
	//		Player->PlayerDeSpawn();
	//		Player->NotifyPlayerDeath();
	//		Player->ShowEndGame();
	//	}
	//	DeSpawn();
	//}
}