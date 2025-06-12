// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "NavigationSystem.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <PlayerWeapon.h>
#include "Components/CapsuleComponent.h"
#include <EnemyCharacter.h>
#include <PlayerHUD.h>
#include "InputManager.h"

#include "GameFramework/PlayerController.h"

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

	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
	CharacterCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerController == nullptr)
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

	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
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
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 항상 스폰허용

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
	// 1회성 충돌이므로, 체력의 감소는 플레이어에서 처리함.
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (IsValid(Enemy))
	{
		float CurrentHp = this->GetHp();
		float PlayerHp = CurrentHp - (Enemy->GetAtk() - this->GetDef());
		if (PlayerHp > 0)
		{
			SetHp(PlayerHp);
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

void APlayerCharacter::ApplyEffectItem(EItemEffectData Data)
{
	//플레이어가 아이템을 파괴 햇을 때, 아이템 효과를 적용받는 함수.
	//Switch를 활용하여 Data에 들어있는 값으로 효과 적용
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

//////////////////////////////////////////////////////////////////////////////
//컨트롤러 매핑 함수
/*
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	//if exist Controller, is bound inputMappingContext
	if (PlayerController != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
	
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (PlayerController != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput((LookAxisVector.X)*-1);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (Weapon && bMouseClickEnable)
	{
		Weapon->Fire(GetAtk());
	} 
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player Weapon inValid"));
		return;
	}
}

void APlayerCharacter::ToggleMap(const FInputActionValue& Value)
{
	if (PlayerController != nullptr)
	{
		APlayerHUD* MyHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (MyHUD != nullptr)
		{
			MyHUD->ToggleMapSelect();         // HUD 쪽 함수 호출

		}
	}
}

void APlayerCharacter::PlayerStat(const FInputActionValue& Value)
{

	if (GetWorld()->GetMapName().Contains("Map"))
	{
		if (PlayerController != nullptr)
		{
			APlayerHUD* MyHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if (MyHUD != nullptr)
			{
				MyHUD->PlayerStateShow();         // HUD 쪽 함수 호출
			}
		}
	}	
	
}

void APlayerCharacter::Click(const FInputActionValue& Value)
{

}
*/
//////////////////////////////////////////////////////////////////////////////


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