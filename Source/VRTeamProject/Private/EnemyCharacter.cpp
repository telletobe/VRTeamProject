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

	SetCurrentHp(50);
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


	NavGenerationRadius = 6000.0f;
	NavRemovalRadius = 500.0f;
	NavInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);


	UCapsuleComponent* EnemyCollision = GetCapsuleComponent();
	EnemyCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	USkeletalMeshComponent* EnemyMesh = GetMesh();
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		OnEnemyAttack.AddDynamic(Player, &APlayerCharacter::TakenDamage);
		Player->OnPlayerDeath.AddDynamic(this,&AEnemyCharacter::DeSpawn);
	}


	if (EnemyMesh && EnemyMesh->GetMaterial(0))
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(EnemyMesh->GetMaterial(0), this);
		EnemyMesh->SetMaterial(0, DynamicMaterial);
	}
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
	TArray<AActor*> FoundPoint;
	TArray<AActor*> RandomPoint;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundPoint);

	if (FoundPoint.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("FindSpawnPoint failed: No ATargetPoint actors in level!"));
		return;
	}

	for (const auto& StartPoint : FoundPoint)
	{
		if (StartPoint && StartPoint->ActorHasTag(TEXT("EnemySpawnPoint")))
		{
			RandomPoint.Add(StartPoint);
		}
	}

	if (RandomPoint.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("FindSpawnPoint failed: No points with tag 'EnemySpawnPoint' found!"));
		return;
	}

	const int32 Index = FMath::RandRange(0, RandomPoint.Num() - 1);
	SetSpawnPoint(RandomPoint[Index]);

	UE_LOG(LogTemp, Warning, TEXT("SpawnPoint assigned: %s"), *SpawnPoint->GetName());

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

	GetMesh()->SetSimulatePhysics(false);
	SetActorHiddenInGame(true);
	bIsActive = false;
	SetActorTickEnabled(false);
}

void AEnemyCharacter::Spawn()
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	GetMesh()->SetSimulatePhysics(true);
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

void AEnemyCharacter::BroadcastEnemyKilled()
{
	OnEnemyDie.Broadcast();
}

void AEnemyCharacter::BoradCastEnemyAttack()
{
	OnEnemyAttack.Broadcast(GetAtk());
}

void AEnemyCharacter::EnemyDeathAnimEnded()
{
	bIsDeathAnim = false;
	DeSpawn();
}

void AEnemyCharacter::PlayHitEffect()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("HitBlend"), 0.5f);

		FTimerHandle ColorResetTimerHandle;
		GetWorldTimerManager().SetTimer(ColorResetTimerHandle, [this]()
			{
				if (DynamicMaterial)
				{
					DynamicMaterial->SetScalarParameterValue(TEXT("HitBlend"), 0.0f);
				}
			}, 0.2f, false); 
	}
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

void AEnemyCharacter::SetSpawnPoint(AActor* TargetPoint)
{
	SpawnPoint = TargetPoint;
}

void AEnemyCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerBulletActor* Bullet = Cast<APlayerBulletActor>(OtherActor);

	if (IsValid(Bullet))
	{
		if (GetCurrentHp() > 0)
		{
			PlayHitEffect();
			PlayHitMontage(); // �ǰ� ���� �߰�
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
					BroadcastEnemyKilled();
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

	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
	EnemyController->StopMovement();

	if (AttackMontage)
	{
		PlayAttackMontage();

		float Duration = AttackMontage->GetPlayLength(); // 애니메이션 길이

		GetWorldTimerManager().SetTimer(AttackAnimTimerHandle, [this, Player]()
			{
				Player->TakenDamage(GetAtk());
				// 4. 적 비활성화
				DeSpawn();

			}, Duration, false);
	}

}