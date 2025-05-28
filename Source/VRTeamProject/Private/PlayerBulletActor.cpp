// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBulletActor.h"
#include "Components/SphereComponent.h"
#include <EnemyCharacter.h>

// Sets default values
APlayerBulletActor::APlayerBulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Asset Data Load
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BulletMeshData(TEXT("'/Engine/EditorMeshes/EditorSphere.EditorSphere'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BulletMaterialData(TEXT("'/Engine/MapTemplates/Materials/BasicAsset03.BasicAsset03'"));

	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BulletCollision"));
	SetRootComponent(BulletCollision);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->AttachToComponent(BulletCollision, FAttachmentTransformRules::KeepRelativeTransform);

	if (BulletMeshData.Succeeded())
	{
		BulletMesh->SetStaticMesh(BulletMeshData.Object);
	}

	if (BulletMaterialData.Succeeded())
	{
		BulletMesh->SetMaterial(0,BulletMaterialData.Object);
	}

	BulletCollision->SetRelativeScale3D(FVector(0.25f));
	BulletMesh->SetRelativeScale3D(FVector(0.25f));

	//총알 메쉬의 충돌을 무시
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//총알 콜리전 충돌 활성화
	BulletCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);



}


void APlayerBulletActor::BulletMove()
{
	
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * MoveInterval;
	SetActorLocation(NewLocation);
}

void APlayerBulletActor::SetDamage(float BulletDamage)
{
	Damage = BulletDamage;
}




// Called when the game starts or when spawned
void APlayerBulletActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BulletCollision))
	{
		BulletCollision->OnComponentBeginOverlap.AddDynamic(this,&APlayerBulletActor::OnBeginOverlap);
	}

	FTimerHandle MoveTimerHandle;
	FTimerHandle DestroyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle,this,&APlayerBulletActor::BulletMove, MoveInterval,true);

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateLambda([this]() {
		if (IsValid(this))
		{
			Destroy();
		}
		}),
		2.0F,false);

	

}

void APlayerBulletActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);

	if (IsValid(Enemy))
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Blue,TEXT("BulletActor : OnBeginOverlap"));

		if(Enemy->GetHp() > 0)
		{
			float EnemyHp = Enemy->GetHp() - (GetDamage()-Enemy->GetDef());
			if(EnemyHp > 0)
			{ 
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("EnemyHp : %.1f"), EnemyHp));
				Enemy->SetHp(EnemyHp);
			}
			else
			{
				Enemy->Destroy();
				GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Blue,TEXT("Enemy Destory!"));

			}

		}
		Destroy();
	}
	else
	{
		return;
	}

}

// Called every frame
void APlayerBulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



