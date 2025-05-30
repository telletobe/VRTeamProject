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

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Data Asset
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObject(TEXT("'/Game/Map/System/Input/IMC_InputMappingContext.IMC_InputMappingContext'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveObject(TEXT("'/Game/Map/System/Input/IA_Move.IA_Move'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> LookObject(TEXT("'/Game/Map/System/Input/IA_Look.IA_Look'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackObject(TEXT("'/Game/Map/System/Input/IA_Attack.IA_Attack'"));

	if (IMCObject.Succeeded())
	{
		IMC_InputMappingContext = IMCObject.Object;
	}

	if (MoveObject.Succeeded())
	{
		IA_Move = MoveObject.Object;
	}

	if (LookObject.Succeeded())
	{
		IA_Look = LookObject.Object;
	}

	if (AttackObject.Succeeded())
	{
		IA_Attack = AttackObject.Object;
	}

	bIsArrived = false;

	SetHp(10.0f);
	SetAtk(5);
	SetDef(1);

	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
	CharacterCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	//if exist Controller, Use SubSystem with EnhancedInputLocalPlayerSubSystem.  and MappingContext.
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_InputMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context Added!"));
		}
	}

	UCapsuleComponent* CharacterCollision = GetCapsuleComponent();
	if (IsValid(CharacterCollision))
	{
		CharacterCollision->OnComponentHit.AddDynamic(this,&APlayerCharacter::OnComponentHit);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("PlayerCharater BeginOverlap Binding succed"));

	} 
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1,3.0f,FColor::Orange,TEXT("PlayerCharater BeginOverlap Binding Fail"));
	}

	//////////////////////////////////////////////////////////////////////////////////////
	/*
	//* 게임 시작 시, TargetPoint를 찾아서 플레이어에게 등록 해줌.
	//* 
	TArray<AActor*> FoundEndPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATargetPoint::StaticClass(),FoundEndPoint);

	for (auto PlayerPoint : FoundEndPoint)
	{
		if (Cast<ATargetPoint>(PlayerPoint))
		{
			if (!EndPoint)
			{
				if (PlayerPoint->ActorHasTag(TEXT("PlayerEndPoint")))
				{
					EndPoint = PlayerPoint;
				}

			}
		}	
	}
	*/
	//////////////////////////////////////////////////////////////////////////////////////
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
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Enemy InComming!"));
		float CurrentHp = this->GetHp();
		float PlayerHp = CurrentHp - (Enemy->GetAtk() - this->GetDef());
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, FString::Printf(TEXT("EnemyATk: %.1f"), Enemy->GetAtk()));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, FString::Printf(TEXT("PlayerHp: %.1f"),PlayerHp));
		if (PlayerHp > 0)
		{
			SetHp(PlayerHp);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Player Die!"));
			Weapon->Destroy();
			Destroy();
		}

		Enemy->DeSpawn();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("PlayerCharacter OnBeginOverLap : Fail To Cast!"));
		return;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////////////////////////////////////////////////////////////////
	/*
	설정된 TargetPoint가 존재할 경우 TargetPoint로 이동합니다. 
	if(!bIsArrived) MoveTargetPoint(EndPoint);
	*/
	////////////////////////////////////////////////////////////////
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//use EnhancedInputComponent, And Binding Action

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
	}

}

//////////////////////////////////////////////////////////////////////////////////////
/*
* 플레이어 캐릭터에게 TargetPoint가 있다면 TargetPoint로 이동합니다.
* 
void APlayerCharacter::MoveTargetPoint(AActor* TargetPoint)
{
	if (!TargetPoint)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Playercharacter.cpp : TargetPoint Doesn't exist!"));
		return;
	}

	FVector TargetPointLocation = TargetPoint->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	if (!PlayerController)
	{
		PlayerController = Cast<APlayerController>(GetController());
	}

	FVector Direction = TargetPointLocation - CurrentLocation;
	Direction.Normalize();

	float Distance = FVector::Dist2D(CurrentLocation, TargetPointLocation);
	if (Distance < 50.0f)
	{
		UE_LOG(LogTemp,Warning,TEXT("Arrived Target!"));
		bIsArrived = true;
		return;
	}

	AddMovementInput(Direction, 1.0f,true);

}
*/

//////////////////////////////////////////////////////////////////////////////////////////////

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
	case EItemEffectData::AppendWeapon:
		break;
	default:
		break;
	}

}

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
	if (Weapon)
	{
		Weapon->Fire(GetAtk());
	} 
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player Weapon inValid"));
		return;
	}
}


void APlayerCharacter::SetHp(float PlayerHp)
{
	Hp = PlayerHp;
}

void APlayerCharacter::SetAtk(float PlayerAtk)
{
	Atk = PlayerAtk;
}

void APlayerCharacter::SetDef(float PlayerDef)
{
	Def = PlayerDef;
}

void APlayerCharacter::SetExp(float PlayerExp)
{
	Exp = PlayerExp;
}