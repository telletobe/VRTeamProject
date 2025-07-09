// Fill out your copyright notice in the Description page of Project Settings.


#include "InputManager.h"
#include "GameFramework/PlayerController.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "PlayerCharacter.h"
#include <PlayerHUD.h>
#include "InputActionValue.h"
#include "Components/WidgetComponent.h"
#include "MapSelectWidget.h"
#include "PlayerStateWidget.h"
#include "StageInfoWidget.h"
#include "Components/WidgetInteractionComponent.h"
#include <Kismet/GameplayStatics.h>

/*
	�������� �Է��� �����ϴ� Ŭ����
*/

TObjectPtr<UInputManager> UInputManager::Instance = nullptr;

UInputManager::UInputManager()
{
	//Data Asset
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObject(TEXT("'/Game/Map/System/Input/IMC_InputMappingContext.IMC_InputMappingContext'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveObject(TEXT("'/Game/Map/System/Input/IA_Move.IA_Move'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> LookObject(TEXT("'/Game/Map/System/Input/IA_Look.IA_Look'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackObject(TEXT("'/Game/Map/System/Input/IA_Attack.IA_Attack'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleMapObject(TEXT("'/Game/Map/System/Input/IA_ToggleMap.IA_ToggleMap'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> PlayerStatObject(TEXT("'/Game/Map/System/Input/IA_PlayerStat.IA_PlayerStat'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ClickObject(TEXT("'/Game/Map/System/Input/IA_Click.IA_Click'"));

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

	if (ToggleMapObject.Succeeded())
	{
		IA_ToggleMap = ToggleMapObject.Object;
	}

	if (PlayerStatObject.Succeeded())
	{
		IA_PlayerStat = PlayerStatObject.Object;
	}

	if (ClickObject.Succeeded())
	{
		IA_Click = ClickObject.Object;
	}
}

void UInputManager::ToggleWidgetVisibility(UWidgetComponent* Widget)
{
	if (!IsValid(Widget))
	{
		return;
	}

	if (Widget->IsVisible())
	{
		Widget->SetVisibility(false);
		Player->SetVisibleRazerMesh(false);

	}
	else
	{
		Widget->SetVisibility(true);
		Player->SetVisibleRazerMesh(true);
	}
}

UInputManager* UInputManager::GetInstance() {
	if (!Instance)
	{
		Instance = NewObject<UInputManager>();
		Instance->AddToRoot();
	}
	return Instance.Get();
}

void UInputManager::Initialize(APlayerCharacter* PlayerCharacter, APlayerController* PC)
{

	Player = PlayerCharacter;

	if (PC)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_InputMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context Added!"));
		}

		if(!IsValid(MyHUD.Get())) MyHUD = Cast<APlayerHUD>(PC->GetHUD());
	}
}

//��Ʈ�ѷ� ���� �Լ�
void UInputManager::BindAction(UEnhancedInputComponent* InputComponent)
{
	if (!InputComponent || !Player) return;

	InputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UInputManager::Move);
	InputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UInputManager::Look);
	InputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &UInputManager::Attack);
	InputComponent->BindAction(IA_ToggleMap, ETriggerEvent::Started, this, &UInputManager::ToggleMap);
	InputComponent->BindAction(IA_PlayerStat, ETriggerEvent::Started, this, &UInputManager::PlayerStat);
	InputComponent->BindAction(IA_Click, ETriggerEvent::Started, this, &UInputManager::onRightTriggerPressed);
	InputComponent->BindAction(IA_Click, ETriggerEvent::Completed, this, &UInputManager::onRightTriggerReleased);

	UE_LOG(LogTemp, Warning, TEXT("BindAction Call"));
}

void UInputManager::Move(const FInputActionValue& Value)
{
	const APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (PlayerController != nullptr)
	{
		PlayerController->GetPawn()->AddMovementInput(PlayerController->GetPawn()->GetActorRightVector(), MovementVector.X);
	}
}

void UInputManager::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	const APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());

	if (PlayerController != nullptr)
	{
		// add yaw and pitch input to controller
		PlayerController->GetPawn()->AddControllerYawInput((LookAxisVector.X) * -1);
		PlayerController->GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UInputManager::Attack(const FInputActionValue& Value)
{
	if (Player->GetWeapon() && Player->IsMouseClickedEnable())
	{
		Player->GetWeapon()->Fire(Player->GetAtk());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Weapon inValid"));
	}
}

void UInputManager::ToggleMap(const FInputActionValue& Value)
{
	// VR
	if (IsValid(Player))
	{
		if (Player->IsActive())
		{
		UWidgetComponent* UserWidgetComp = Player->GetWidgetComponent();
		UMapSelectWidget* MapSelectInstance = MyHUD->GetMapSelectInstance();
		
			if (IsValid(UserWidgetComp))
			{
				if (UserWidgetComp->GetUserWidgetObject() == MapSelectInstance) 
				{
					ToggleWidgetVisibility(UserWidgetComp);

				}
				else 
				{
					UserWidgetComp->SetWidget(MapSelectInstance);
					if (!UserWidgetComp->IsWidgetVisible())
					{
						ToggleWidgetVisibility(UserWidgetComp);
						UE_LOG(LogTemp, Warning, TEXT("Player Map"));
					}
				}
			}
		} 
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is Dead"));
		}
	}

}

void UInputManager::PlayerStat(const FInputActionValue& Value)
{
	//VR
	if (IsValid(Player))
	{
		if (Player->IsActive())
		{
			UWidgetComponent* UserWidgetComp = Player->GetWidgetComponent();
			UPlayerStateWidget* PlayerStateInstance = MyHUD->GetPlayerStateInstance();

			if (IsValid(UserWidgetComp))
			{
				if (UserWidgetComp->GetUserWidgetObject() == PlayerStateInstance)
				{
					ToggleWidgetVisibility(UserWidgetComp);
					
				}
				else
				{
					UserWidgetComp->SetWidget(PlayerStateInstance);
					if (!UserWidgetComp->IsWidgetVisible())
					{
						UE_LOG(LogTemp, Warning, TEXT("Player Stat"));
						ToggleWidgetVisibility(UserWidgetComp);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Is Dead"));
		}
	}
}

void UInputManager::onRightTriggerPressed()
{
	if (IsValid(Player))
	{
		UWidgetInteractionComponent* WidgetInteractionRight = Player->GetWidgetInteractionRight();
		if (WidgetInteractionRight && WidgetInteractionRight->IsOverFocusableWidget())
		{
			WidgetInteractionRight->PressPointerKey(EKeys::LeftMouseButton);
		}
	}
}

void UInputManager::onRightTriggerReleased()
{
	if (IsValid(Player))
	{
		UWidgetInteractionComponent* WidgetInteractionRight = Player->GetWidgetInteractionRight();
		if (WidgetInteractionRight && WidgetInteractionRight->IsOverFocusableWidget())
		{
			WidgetInteractionRight->ReleasePointerKey(EKeys::LeftMouseButton);
		}
	}
}
