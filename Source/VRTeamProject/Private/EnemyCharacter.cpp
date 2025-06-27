// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NavigationInvokerComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include <EnemyAIController.h>
#include "PlayerCharacter.h"
#include "PlayerBulletActor.h"

/*
	Enemy class 의메모리의 할당은 Spawner 클래스에서 담당함.
	메모리 해제는 게임모드에서 관리
*/

AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//기본 값 세팅
	SetCurrentHp(1);
	SetDef(1.0f);
	SetAtk(3.0f);
	SetSpawnDelay(2.0f);

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


	if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		OnEnemyAttack.AddDynamic(Player, &APlayerCharacter::TakenDamage);
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
	////Enemy가 활성될 때 위치할 곳
	TArray<AActor*> FoundPoint;
	TArray<AActor*> RandomPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundPoint);

	for (const auto& StartPoint : FoundPoint)
	{
		if (Cast<ATargetPoint>(StartPoint))
		{		
			UE_LOG(LogTemp, Warning, TEXT("Found point"));
			if (StartPoint->ActorHasTag(TEXT("EnemySpawnPoint")))
			{
				SpawnPoint = StartPoint;
				RandomPoint.Add(StartPoint);
			}	
		}
	}
	if (RandomPoint.Num() > 0)
	{
		SpawnPoint = RandomPoint[FMath::RandRange(0,RandomPoint.Num()-1)];
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("no valid spawn points with tag 'EnemySpawnPoint' found"));
	}
}

void AEnemyCharacter::FindDeSpawnPoint()
{
	//Enemy가 비활성 될 때 이동할 곳
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
	//메모리에서 제거하지않고 비활성으로 전환
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
	//메모리에 상주중인 데이터를 활성상태로 변경
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
	OnEnemyKilled.Broadcast();
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

	if (IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player IsValid"));
		float PlayerCurrentHp = Player->GetHp();
		float PlayerHp = PlayerCurrentHp - (GetAtk() - Player->GetDef());

		BoradCastEnemyAttack();
		DeSpawn();
	}
}