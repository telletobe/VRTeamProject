// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NavigationInvokerComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerBulletActor.h"
#include "System/VRProjectGameModeBase.h"


AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetCurrentHp(GetMaxHp());
	SetDef(DefaultDef); //1
	SetAtk(DefaultAtk); //5

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	UE_LOG(LogTemp,Warning,TEXT("EnemyCharacter : BeginPlay Call"));

	Super::BeginPlay();

	NavGenerationRadius = 6000.0f;
	NavRemovalRadius = 500.0f;
	NavInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);

	UCapsuleComponent* EnemyCollision = GetCapsuleComponent();
	EnemyCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	USkeletalMeshComponent* EnemyMesh = GetMesh();
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
	if (CharacterCollision)
	{
		CharacterCollision->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnComponentHit);
		CharacterCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnBeginOverlap);
		OnEnemyDeathAnimEnded.AddDynamic(this, &AEnemyCharacter::EnemyDeathAnimEnded);	
	}

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		OnEnemyAttack.AddDynamic(Player, &APlayerCharacter::TakenDamage);
		Player->OnPlayerDeath.AddDynamic(this,&AEnemyCharacter::DeSpawn);
	}

	AVRProjectGameModeBase* GM = Cast<AVRProjectGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		OnEnemyDie.AddDynamic(GM, &AVRProjectGameModeBase::OnEnemyDeath);
	}

	if (EnemyMesh && EnemyMesh->GetMaterial(0))
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(EnemyMesh->GetMaterial(0), this);
		EnemyMesh->SetMaterial(0, DynamicMaterial);
	}
	FindSpawnPoint();
	FindDeSpawnPoint();

	TArray<AActor*> WeatherManagerActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeatherManager::StaticClass(), WeatherManagerActor);

	for (const auto& FoundActor : WeatherManagerActor)
	{
		if (FoundActor)
		{
			AWeatherManager* WeatherManager = Cast<AWeatherManager>(FoundActor);
			if (WeatherManager)
			{
				ApplyWeatherEffect(WeatherManager->GetWeatherData());
			}
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("enemycharacter : NotFound WeatherManager!"));
		}
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

void AEnemyCharacter::SetCurrentHp(const float EnemyHp)
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

void AEnemyCharacter::SetMaxHp(const float EnemyHp)
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

void AEnemyCharacter::SetDef(const float EnemyDef)
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

void AEnemyCharacter::SetAtk(const float EnemyAtk)
{
	if (EnemyAtk < 0)
	{
		Atk = DefaultAtk;
	}
	else
	{
		Atk = EnemyAtk;
	}
}

// Spawn 관련코드

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
		FindDeSpawnPoint();
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
	}

	return;
}

void AEnemyCharacter::SetSpawnPoint(AActor* TargetPoint)
{
	SpawnPoint = TargetPoint;
}

// 델리게이트
void AEnemyCharacter::BroadcastEnemyKilled()
{
	OnEnemyDie.Broadcast();
}

void AEnemyCharacter::BoradCastEnemyAttack()
{
	OnEnemyAttack.Broadcast(GetAtk());
}

// 애니메이션
void AEnemyCharacter::EnemyDeathAnimEnded()
{
	if (IsValid(this)) {
		bIsDeathAnim = false;
		DeSpawn();
	}	
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
				AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
				EnemyController->StopMovement();
				AnimInstance->Montage_Play(DeathMontage);
				bIsDeathAnim = true;
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

//날씨에 따른 능력치 변경

void AEnemyCharacter::ApplyWeatherEffect(EWeatherData Weather)
{
	switch (Weather)
	{
	case EWeatherData::SUN:
		GetCharacterMovement()->MaxWalkSpeed = 450.0f;
		SetMaxHp(50.0f);
		SetAtk(DefaultAtk);
		SetDef(DefaultDef);
		break;
	case EWeatherData::RAIN:
		GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
		SetMaxHp(20.0f);
		SetAtk(3.0);
		SetDef(0.0f);
		break;
	case EWeatherData::FOGGY:
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		SetMaxHp(60.0f);
		SetAtk(20.0);
		SetDef(3.0f);
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = 450.0f;
		SetMaxHp(50.0f);
		SetAtk(DefaultAtk);
		SetDef(DefaultDef);
		break;
	}
}

//콜리전 반응
void AEnemyCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerBulletActor* Bullet = Cast<APlayerBulletActor>(OtherActor);

	if (IsValid(Bullet))
	{
		if (GetCurrentHp() > 0)
		{
			PlayHitEffect();
			PlayHitMontage();
			const float finalDamage = FMath::Max(1.0f, Bullet->GetDamage()-GetDef());
			float EnemyHp = GetCurrentHp() - finalDamage;
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
					SetActorEnableCollision(false);
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

	if (Player)
	{
		AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController());
		EnemyController->StopMovement();
		if (AttackMontage)
		{
			PlayAttackMontage();
			float Duration = AttackMontage->GetPlayLength(); // 애니메이션 길이
			GetWorldTimerManager().SetTimer(AttackAnimTimerHandle, [this]()
				{
					BoradCastEnemyAttack();
					// 4. 적 비활성화
					DeSpawn();
				}, Duration, false);
		}
	}
}