// Fill out your copyright notice in the Description page of Project Settings.


#include "BulidingBaseActor.h"
#include <Components/BoxComponent.h>
#include <Engine/TargetPoint.h>

// Sets default values
ABulidingBaseActor::ABulidingBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulidingCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BulidingCollision"));
	SetRootComponent(BulidingCollision);

	BulidingCollision->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));

	BulidRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	BulidLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	BulidMiddle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Middle"));

	BulidRight->AttachToComponent(BulidingCollision,FAttachmentTransformRules::KeepRelativeTransform);
	BulidLeft->AttachToComponent(BulidingCollision, FAttachmentTransformRules::KeepRelativeTransform);
	BulidMiddle->AttachToComponent(BulidingCollision, FAttachmentTransformRules::KeepRelativeTransform);

	MoveForce = FVector(-1200, 0, 0);

}

void ABulidingBaseActor::SetIsArrive(bool IsArrive)
{
	bIsArrive = IsArrive;
}

// Called when the game starts or when spawned
void ABulidingBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!EndPoint)
	{
		UE_LOG(LogTemp,Warning,TEXT("EndPoint is NULL! Please Set Engine"));
		return;
	}

	StartLocation = GetActorLocation();

}

void ABulidingBaseActor::MovetoTarget(ATargetPoint* End,float DeltaTime)
{

	float Distance = FVector::Dist2D(GetActorLocation(), EndPoint->GetActorLocation()); // 건물과 타겟포인트의 거리
	if (Distance < 200.0f)
	{
		SetIsArrive(true);
		return;
	}
																	  // MoveForce 값에 따라 이동속도조절
	FVector NewLocation = GetActorLocation() + MoveForce * DeltaTime; //DelataTime을 활용하여 자연스러운 움직임 처리
	SetActorLocation(NewLocation);

}

void ABulidingBaseActor::BackToStartPoint(FVector Location)
{
	this->SetActorLocation(Location);
	SetIsArrive(false);
}

// Called every frame
void ABulidingBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsArrive)
	{
		MovetoTarget(EndPoint,DeltaTime);
	}
	else
	{
		BackToStartPoint(StartLocation);
	}
}