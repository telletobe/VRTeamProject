// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "NavigationSystem.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Data Asset
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObject(TEXT("'/Game/Map/System/Input/IMC_InputMappingContext.IMC_InputMappingContext'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveObject(TEXT("'/Game/Map/System/Input/IA_Move.IA_Move'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> LookObject(TEXT("'/Game/Map/System/Input/IA_Look.IA_Look'"));

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

	bIsArrived = false;

	SetHp(100.0f);
	SetAtk(5);
	SetDef(1);

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

	TArray<AActor*> FoundEndPoint;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATargetPoint::StaticClass(),FoundEndPoint);

	for (auto PlayerPoint : FoundEndPoint)
	{
		EndPoint = Cast<ATargetPoint>(PlayerPoint);
		if (!EndPoint)
		{
			EndPoint->ActorHasTag(TEXT("PlayerEndPoint"));
		}
	}


}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsArrived) MoveTargetPoint(EndPoint);

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
	}

}

void APlayerCharacter::MoveTargetPoint(AActor* TargetPoint)
{
	if (!TargetPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetPoint Doesn't exist!"));
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