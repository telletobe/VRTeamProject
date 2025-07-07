// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <EnhancedInputComponent.h>
#include "Components/CapsuleComponent.h"
#include <PlayerWeapon.h>
#include "InputManager.h"
#include "MotionControllerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnemyCharacter.h"
#include "EngineUtils.h"
// Sets default values
// 커밋용 주석추가
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetHp(100.0f);
	SetAtk(5.0f);
	SetDef(1);
	bIsActive = false;
	bMouseClickEnable = false;
	//VR

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(GetCapsuleComponent());
	SceneComponent->SetRelativeLocation(FVector(20.0f,0,55.0f));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(SceneComponent);
	VRCamera->bLockToHmd = true;
	VRCamera->SetRelativeLocation(FVector(FVector::ZeroVector));
	VRCamera->SetRelativeScale3D(FVector(0.25f,0.5f,0.5f));

	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerLeft->SetupAttachment(SceneComponent);
	MotionControllerLeft->SetTrackingSource(EControllerHand::Left);

	WidgetInteractionLeft = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionLeft"));
	WidgetInteractionLeft->SetupAttachment(MotionControllerLeft);
	WidgetInteractionLeft->InteractionDistance = 10000.0f; // 위젯 반응거리
	WidgetInteractionLeft->PointerIndex = 0; //왼쪽 입력 구분
	WidgetInteractionLeft->bShowDebug = true;

	MotionControllerLeftLazerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotionControllerLeftLazerMesh"));
	MotionControllerLeftLazerMesh->SetupAttachment(WidgetInteractionLeft);
	MotionControllerLeftLazerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MotionControllerLeftLazerMesh->SetRelativeLocation(FVector(FVector::ZeroVector));

	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	MotionControllerRight->SetupAttachment(SceneComponent);
	MotionControllerRight->SetTrackingSource(EControllerHand::Right);

	WidgetInteractionRight = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionRight"));
	WidgetInteractionRight->SetupAttachment(MotionControllerRight);
	WidgetInteractionRight->InteractionDistance = 10000.0f;
	WidgetInteractionRight->PointerIndex = 1; //오른쪽 입력 구분
	WidgetInteractionRight->bShowDebug = true;

	MotionControllerRightLazerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotionControllerRightLazerMesh"));
	MotionControllerRightLazerMesh->SetupAttachment(WidgetInteractionRight);
	MotionControllerRightLazerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MotionControllerRightLazerMesh->SetRelativeLocation(FVector(FVector::ZeroVector));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(VRCamera);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetDrawSize(FVector2D(768.0f,1150.0f));
	WidgetComponent->SetRelativeLocation(FVector(200.0f,0.0f,0.0f));
	WidgetComponent->SetVisibility(false);
	
}

void APlayerCharacter::SetVisibleRazerMesh(bool visible)
{
	if (MotionControllerRightLazerMesh || MotionControllerLeftLazerMesh)
	{
		MotionControllerRightLazerMesh->SetVisibility(visible);
		MotionControllerLeftLazerMesh->SetVisibility(visible);
	}
}

void APlayerCharacter::InVisibleRezerMesh()
{
	if (MotionControllerRightLazerMesh || MotionControllerLeftLazerMesh)
	{
		MotionControllerRightLazerMesh->SetVisibility(false);
		MotionControllerLeftLazerMesh->SetVisibility(false);
	}
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
	
		if (IsValid(PlayerController))
		{
			UInputComponent* InputComp = PlayerController->InputComponent;
			if (!InputManager)
			{
				InputManager = InputManager->GetInstance();
				InputManager->Initialize(this, PlayerController);
				InputManager->BindAction(Cast<UEnhancedInputComponent>(InputComp));
			}
		}
		if (MotionControllerLeftLazerMesh)
		{
			MotionControllerLeftLazerMesh->AttachToComponent(WidgetInteractionLeft, FAttachmentTransformRules::KeepRelativeTransform);
		}

		if (MotionControllerRightLazerMesh)
		{
			MotionControllerRightLazerMesh->AttachToComponent(WidgetInteractionRight, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
	if (!bIsActive) //기본값 false
	{
		PlayerReSpawn();
	}

	if (!Weapon)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 항상 스폰허용

		APlayerWeapon* NewWeapon = GetWorld()->SpawnActor<APlayerWeapon>(APlayerWeapon::StaticClass(), FVector(0), FRotator(0),SpawnParams);
		if (IsValid(NewWeapon))
		{
			Weapon = NewWeapon;
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RightHand");
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerWeapon InValid"));
	}

	if (GetHp() < 0)
	{
		SetHp(GetMaxHp());
	}
	if (GetAtk() < 0) {
		SetAtk(GetDefaultAtk());
	}
	if (GetDef() < 0)
	{
		SetDef(GetDefaultDef());
	}
	if (GetExp() < 0)
	{
		SetExp(0.0f);
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
	//플레이어가 아이템을 파괴 햇을 때, 아이템 효과를 적용받는 함수.
	//Switch를 활용하여 Data에 들어있는 값으로 효과 적용
	GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Green,TEXT("Player ApplyEffetItem!"));
	FTimerHandle RestoreTimerHandle;
	
	switch (Data)
	{
	case EItemEffectData::HEAL:
		// 예: 체력 20 회복
		SetHp(GetHp() + 20);
		if (GetHp() > GetMaxHp())
		{
			SetHp(GetMaxHp());
		}
		NotifyPlayerChangeHealth();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("HEAL applied: +20 HP"));
		break;

	case EItemEffectData::AtkUp:
		
		SetAtk(GetAtk() + 1);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("AtkUp applied: Atk = %.1f (for 10 seconds)"), GetAtk()));

		GetWorldTimerManager().ClearTimer(RestoreTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, [this]() {SetAtk(DefaultAtk);}, 10.0f, false);	
		break;

	case EItemEffectData::DefUp:
		SetDef(GetDef() + 1);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("DefUp applied: Def = %.1f"), GetDef()));

		GetWorld()->GetTimerManager().ClearTimer(RestoreTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, [this]() {SetDef(DefaultDef);},10.0f, false);
		break;

	case EItemEffectData::AttackSpeed:

		if (!IsValid(Weapon))
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerWeapon is nullptr in AttackSpeed effect"));
			return;
		}
		else
		{
			GetWeapon()->SetFireDelay(0.005f);
		}

		GetWorld()->GetTimerManager().ClearTimer(RestoreTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, [this]() { GetWeapon()->SetFireDelay(GetWeapon()->GetDefaultFireDelay()); }, 10.0f, false);
		break;

	default :
		break;
	}

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

	if (WidgetComponent->IsWidgetVisible())
	{
		WidgetComponent->SetVisibility(false);
		InVisibleRezerMesh();
	}

	SetHp(GetMaxHp());

	if (Weapon) Weapon->SetActorHiddenInGame(false);
	bIsActive = true;
	SetActorHiddenInGame(false);
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
}

void APlayerCharacter::TakenDamage(float Damage)
{
	float PlayerHp = GetHp();
	if (PlayerHp >= 0)
	{
		SetHp(PlayerHp - (Damage-GetDef()));
		NotifyPlayerChangeHealth();
	}
	else
	{
		NotifyPlayerDeath();
		PlayerDeSpawn();
	}
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