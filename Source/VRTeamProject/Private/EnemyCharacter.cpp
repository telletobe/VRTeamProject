// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"



AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));


	// NavInvoker ���� ����,���� ������ ����
	NavGenerationRadius = 1000.0f;
	NavRemovalRadius = 150.0f;
	NavInvoker->SetGenerationRadii(NavGenerationRadius, NavRemovalRadius);



	//�����ߴٰ� �Ǵ��ϴ� �Ÿ�
	AcceptanceRadius = 100.0f;
	bIsSucceeded = false;
	bIsMoving = false;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	

	AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		AIController->ReceiveMoveCompleted.AddDynamic(this, &AEnemyCharacter::OnMoveCompleted);
	}

	FindTargetPoints();
	StartMoving();

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

void AEnemyCharacter::MoveToTarget()
{
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is not valid!"));
		return;
	}

	if (bIsMoving)
	{
		return;
	}

	AActor* SelectedTarget = bIsSucceeded ? Target : Target2;

	if (SelectedTarget)
	{
		bIsMoving = true;

		//AI MoveTo �Լ� ȣ��
		FVector TargetLocation = SelectedTarget->GetActorLocation();
		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(
			TargetLocation,
			AcceptanceRadius,
			true,
			true,
			true);

		if (MoveResult == EPathFollowingRequestResult::Failed)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Start Movement to Target!"));
			bIsMoving = false;
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Moving to %s (IsSucceeded : %s"), *SelectedTarget->GetName(), bIsSucceeded ? TEXT("True") : TEXT("False"));
			bIsMoving = false;
		}
	}

}

void AEnemyCharacter::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	bIsMoving = false;

	//�̵� ����� ���� isSucceeded �� ���
	if (Result == EPathFollowingResult::Success)
	{
		//�̵� ����!
		bIsSucceeded = !bIsSucceeded;
		UE_LOG(LogTemp, Display, TEXT("Move Completed successfully. IsSucceeded toglled to : %s"), bIsSucceeded ? TEXT("TRUE") : TEXT("FALSE"));

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacter::MoveToTarget, 1.0f, false);

	}
	else
	{
		//�̵� ����

		UE_LOG(LogTemp, Warning, TEXT("Move failed with result : %d"), static_cast<int32>(Result));
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacter::MoveToTarget, 1.0f, false);
	}
}

void AEnemyCharacter::StartMoving()
{
	// Ÿ�� ����Ʈ�� ã�� �̵� ����
	FindTargetPoints();
	MoveToTarget();
}

void AEnemyCharacter::FindTargetPoints()
{
	// Ÿ�� ����Ʈ�� �����Ǿ� ���� ���� ��� �ڵ����� ã��
	if (!(Target || Target2))
	{
		TArray<AActor*> FoundTargets;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), FoundTargets);

		if (FoundTargets.Num() >= 2)
		{
			Target = FoundTargets[0];
			Target2 = FoundTargets[1];
			UE_LOG(LogTemp, Display, TEXT("Found TargetPoints: %s and %s"), *Target->GetName(), *Target2->GetName());

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough TargetPoints found in the level."));
		}
	}

}

