// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <EnhancedInputComponent.h>
#include <PlayerWeapon.h>
#include "Components/CapsuleComponent.h"
#include <EnemyCharacter.h>
#include <PlayerHUD.h>
#include "InputManager.h"
#include "MotionControllerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetHp(10.0f);
	SetAtk(5);
	SetDef(1);
	bIsActive = false;
	bMouseClickEnable = false;



	//VR

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bLockToHmd = true;
	VRCamera->SetRelativeLocation(FVector(5.0f, 0.0f, 64.f)); // �Ӹ� ��ġ��
	VRCamera->SetRelativeScale3D(FVector(0.25f,0.5f,0.5f));

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerLeft->SetupAttachment(RootComponent);
	MotionControllerLeft->SetTrackingSource(EControllerHand::Left);

	MotionControllerLeftLazerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotionControllerLeftLazerMesh"));
	MotionControllerLeftLazerMesh->SetupAttachment(MotionControllerLeft);

	WidgetInteractionLeft = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionLeft"));
	WidgetInteractionLeft->SetupAttachment(MotionControllerLeft);
	WidgetInteractionLeft->InteractionDistance = 5000.0f; // ���� �����Ÿ�
	WidgetInteractionLeft->PointerIndex = 0; //���� �Է� ����
	WidgetInteractionLeft->bShowDebug = true;

	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	MotionControllerRight->SetupAttachment(RootComponent);
	MotionControllerRight->SetTrackingSource(EControllerHand::Right);
	
	MotionControllerRightLazerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotionControllerRightLazerMesh"));
	MotionControllerRightLazerMesh->SetupAttachment(MotionControllerRight);

	WidgetInteractionRight = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionRight"));
	WidgetInteractionRight->SetupAttachment(MotionControllerRight);
	WidgetInteractionRight->InteractionDistance = 5000.0f;
	WidgetInteractionRight->PointerIndex = 1; //������ �Է� ����
	WidgetInteractionRight->bShowDebug = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetCapsuleComponent());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetDrawSize(FVector2D(500.0f,500.0f));
	WidgetComponent->SetRelativeLocation(FVector(200.0f,0.0f,0.0f));


}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
	CharacterCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (!PlayerController)
	{
		PlayerController = Cast<APlayerController>(GetController());
		UInputComponent* InputComp = PlayerController->InputComponent;

		if (!InputManager)
		{
			InputManager = InputManager->GetInstance();
			InputManager->Initialize(this, PlayerController);
			InputManager->BindAction(Cast<UEnhancedInputComponent>(InputComp));
		}
	}

	if (IsValid(CharacterCollision))
	{
		CharacterCollision->OnComponentHit.AddDynamic(this,&APlayerCharacter::OnComponentHit);
	} 

	if (!bIsActive)
	{
		PlayerReSpawn();
	}

	if (!Weapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // �׻� �������

		APlayerWeapon* NewWeapon = GetWorld()->SpawnActor<APlayerWeapon>(APlayerWeapon::StaticClass(), FVector(0), FRotator(0),SpawnParams);
		if (IsValid(NewWeapon))
		{
			Weapon = NewWeapon;
			Weapon->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void APlayerCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 1ȸ�� �浹�̹Ƿ�, ü���� ���Ҵ� �÷��̾�� ó����.
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (IsValid(Enemy))
	{
		float CurrentHp = GetHp();
		float PlayerHp = CurrentHp - (Enemy->GetAtk() - this->GetDef());
		if (PlayerHp > 0)
		{
			SetHp(PlayerHp);
			NotifyPlayerChangeHealth();
		}
		else
		{
			PlayerDeSpawn();
			NotifyPlayerDeath();
		}

		Enemy->DeSpawn();

	}
	else
	{
		return;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::ApplyEffectItem(const EItemEffectData& Data)
{
	//�÷��̾ �������� �ı� ���� ��, ������ ȿ���� ����޴� �Լ�.
	//Switch�� Ȱ���Ͽ� Data�� ����ִ� ������ ȿ�� ����
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,TEXT("Player ApplyEffetItem!"));

	switch (Data)
	{
	case EItemEffectData::HEAL:

		break;
	case EItemEffectData::AtkUp:

		break;
	case EItemEffectData::DefUp:

		break;
	case EItemEffectData::AttackSpeed:

		break;

	}

}

void APlayerCharacter::PlayerDeSpawn()
{
	if (PlayerController) 
	{
		PlayerController->SetIgnoreMoveInput(true);
		bMouseClickEnable = false;
	}
	if (Weapon) Weapon->SetActorHiddenInGame(true);
	bIsActive = false;
	SetActorHiddenInGame(true);

}

void APlayerCharacter::NotifyPlayerDeath()
{
	OnPlayerDeath.Broadcast();
	UE_LOG(LogTemp,Warning,TEXT("NotifyPlayerDeath"));
}

void APlayerCharacter::NotifyPlayerChangeHealth()
{
	OnHealthChange.Broadcast(GetHp(),GetMaxHp());
}

void APlayerCharacter::PlayerReSpawn()
{
	if (PlayerController)
	{
		PlayerController->SetIgnoreMoveInput(false);
		bMouseClickEnable = true;
	}

	if (Weapon) Weapon->SetActorHiddenInGame(false);
	bIsActive = true;
	SetActorHiddenInGame(false);
}

void APlayerCharacter::SetHp(float PlayerHp)
{
	if (PlayerHp < 0)
	{
		Hp = GetMaxHp();
	}
	else
	{
		Hp = PlayerHp;
	}
}

void APlayerCharacter::SetAtk(float PlayerAtk)
{
	if (PlayerAtk < 0)
	{
		Atk = DefaultAtk;
	}
	else
	{
		Atk = PlayerAtk;
	}
}

void APlayerCharacter::SetDef(float PlayerDef)
{
	if (PlayerDef < 0)
	{
		Def = DefaultDef;
	}
	else
	{
		Def = PlayerDef;
	}
}

void APlayerCharacter::SetExp(float PlayerExp)
{
	Exp = PlayerExp;
}