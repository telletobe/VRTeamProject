// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <EnhancedInputComponent.h>
#include "Components/CapsuleComponent.h"
#include "Player/PlayerWeapon.h"
#include "System/InputManager.h"
#include "MotionControllerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"

APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetHp(100.0f);
	SetAtk(15.0f);
	SetDef(1);
	bIsActive = false;
	bMouseClickEnable = false;

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
	WidgetComponent->SetVisibility(false);
	WidgetComponent->SetCastShadow(false);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::SetVisibleRazerMesh(bool visible)
{
	if (MotionControllerRightLazerMesh || MotionControllerLeftLazerMesh)
	{
		MotionControllerRightLazerMesh->SetVisibility(visible);
		MotionControllerLeftLazerMesh->SetVisibility(visible);
	}
}

void APlayerCharacter::HideWidgetComponent()
{
	if (WidgetComponent && WidgetComponent->IsVisible())
	{
		WidgetComponent->SetVisibility(false);
		SetVisibleRazerMesh(false);
	}
}

void APlayerCharacter::SetPlayerLocation(float X)
{
	FVector CurrentLocation = GetActorLocation();

	FVector NewLocation = FVector(X, CurrentLocation.Y+3, CurrentLocation.Z); // 이동할 위치
	SetActorLocation(NewLocation);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
	if (CharacterCollision)
	{
		CharacterCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	if (!PlayerController)
	{
		PlayerController = Cast<APlayerController>(GetController());
	
		if (IsValid(PlayerController))
		{
			UInputComponent* InputComp = PlayerController->InputComponent;
			if (!InputManager && IsValid(InputComp))
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
		PlayerReSpawn(); // 이곳에서 bIsActive를 ture로 변경.
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

	SetPlayerLocation(-9000);
}

// Player Spawn 및 DeSpawn

void APlayerCharacter::PlayerReSpawn()
{
	SetPlayerLocation(-4000.0f);
	if (Weapon) Weapon->SetActorHiddenInGame(false);

	FTimerHandle InputDelayHandle;
	GetWorld()->GetTimerManager().SetTimer(InputDelayHandle, [this]()
		{
			if (PlayerController)
			{
				//PlayerController->SetIgnoreMoveInput(false);
				bMouseClickEnable = true;
			}
		}, 0.1f, false
	);

	HideWidgetComponent();
	SetHp(GetMaxHp());
	NotifyPlayerChangeHealth();
	bIsActive = true;
}

void APlayerCharacter::PlayerDeSpawn()
{
	if (PlayerController)
	{
		//PlayerController->SetIgnoreMoveInput(true);
		bMouseClickEnable = false;
	}
	if (Weapon) Weapon->SetActorHiddenInGame(true);
	bIsActive = false;
}

// 플레이어의 무기 생성

void APlayerCharacter::SpawnWeapon()
{
	if (Weapon) return; // 이미 무기가 있으면 생성하지 않음
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APlayerWeapon* NewWeapon = GetWorld()->SpawnActor<APlayerWeapon>(APlayerWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (IsValid(NewWeapon))
	{
		Weapon = NewWeapon;

		// 오른손 모션 컨트롤러에 부착
		Weapon->AttachToComponent(MotionControllerRight, FAttachmentTransformRules::SnapToTargetIncludingScale);

		// 무기 숨김 해제
		Weapon->SetActorHiddenInGame(false);

		// 기본 Fire Delay 설정 등 초기화 필요 시 여기에 추가
		Weapon->SetFireDelay(Weapon->GetDefaultFireDelay());

		UE_LOG(LogTemp, Log, TEXT("Weapon spawned and attached to player."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn weapon."));
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

// 아이템 적용

void APlayerCharacter::ApplyEffectItem(const EItemEffectData Data)
{
	//플레이어가 아이템을 파괴 햇을 때, 아이템 효과를 적용받는 함수.
	//Switch를 활용하여 Data에 들어있는 값으로 효과 적용
	//공격력 증가와 공격속도 증가만 적용중
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
		
		SetAtk(GetAtk() + 50);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("AtkUp applied: Atk = %.1f (for 10 seconds)"), GetAtk()));

		GetWorldTimerManager().ClearTimer(RestoreTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, [this]() {SetAtk(DefaultAtk);}, 3.0f, false);	
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
		GetWorld()->GetTimerManager().SetTimer(RestoreTimerHandle, [this]() { GetWeapon()->SetFireDelay(GetWeapon()->GetDefaultFireDelay()); }, 4.0f, false);
		break;

	default :
		break;
	}

}

// 플레이어가 적이 공격했다는 BroadCast 신호를 받아서 호출하는 델리게이트

void APlayerCharacter::TakenDamage(float Damage)
{
	
	float PlayerHp = GetHp();
	if (PlayerHp > 0)
	{
		float finalDamage = FMath::Max(1.0f, Damage - GetDef());
		SetHp(PlayerHp - (finalDamage));
		NotifyPlayerChangeHealth();
		if (GetHp() < 0)
		{
			NotifyPlayerDeath();
			PlayerDeSpawn();
			return;
		}
	}
	else
	{
		NotifyPlayerDeath();
		PlayerDeSpawn();
		return;
	}
}

// Setter

void APlayerCharacter::SetHp(const float PlayerHp)
{
	Hp = PlayerHp;
}

void APlayerCharacter::SetAtk(const float PlayerAtk)
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

void APlayerCharacter::SetDef(const float PlayerDef)
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

void APlayerCharacter::SetExp(const float PlayerExp)
{
	Exp = PlayerExp;
}

void APlayerCharacter::SetMaxHp(const float PlayerMaxHp)
{
	if (PlayerMaxHp < 0)
	{
		MaxHp = 100.0f;
	}
	else
	{
		MaxHp = PlayerMaxHp;
	}
}

//플레이어 사망, 위젯 업데이트를 위한 델리게이트호출

void APlayerCharacter::NotifyPlayerDeath()
{
	OnPlayerDeath.Broadcast();
}

void APlayerCharacter::NotifyPlayerChangeHealth()
{
	OnHealthChange.Broadcast(GetHp(), GetMaxHp());
}
