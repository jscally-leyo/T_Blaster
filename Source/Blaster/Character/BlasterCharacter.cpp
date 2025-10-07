// All rights preserved to Leyodemus

#include "BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Blaster/BlasterComponents/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/Weapon/Weapon.h"

#include "Blaster/Helper/BlasterHelperDebug.h"

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->SetWalkableFloorAngle(46.f);

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	CursorSpeed = 2; // Should be a menu setting, 2 is good for the default speed here imho

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true); // Components don't need to be registered in GetLifeTimeReplicatedProps

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// Components don't need to be registered in GetLifeTimeReplicatedProps
	
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register stuff that needs to be replicated - also check UPROPERTY of these in the header file
	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Debug message
	//BlasterHelperDebug::Print(TEXT("Session started, godspeed!")); // This comes from the namespace BlasterHelperDebug in BlasterHelperDebug.h

	// Initialize controller
	PC = Cast<APlayerController>(Controller);

	// Add the input mapping context to the player
	if (PC)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind actions to callback functions here
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Look);
		}

		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ABlasterCharacter::Jump);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABlasterCharacter::StopJumping);
		}

		if (EquipAction)
		{
			EnhancedInput->BindAction(EquipAction, ETriggerEvent::Started, this, &ABlasterCharacter::EquipButtonPressed);
		}
		
		if (CrouchAction)
		{
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABlasterCharacter::CrouchButtonPressed);
		}
	}
}

void ABlasterCharacter::PostInitializeComponents()
{
	// This will make sure the components are "loaded in" so we have access to them (if permitted, like the CombatComponent here because it's a 'friend' class)

	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character = this; // This is the 'earliest' point where we can set the character variable in the Combat component
	}
}

void ABlasterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (PC != nullptr)
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		// Forward/Backward
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		// Right/Left
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	else
	{
		BlasterHelperDebug::Print(TEXT("Player Controller not valid in BlasterCharacter.Move()."));
	}
}

void ABlasterCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (PC != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * CursorSpeed);
		AddControllerPitchInput(LookAxisVector.Y * CursorSpeed);
	}
	else
	{
		BlasterHelperDebug::Print(TEXT("Player Controller not valid in BlasterCharacter.Look()."));
	}
}

void ABlasterCharacter::EquipButtonPressed(const FInputActionValue& Value)
{
	if (Combat) 
	{
		if (HasAuthority())
		{
			// This is the authority version where a server wants to equip
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			// This is the RPC version where a client wants to equip
			ServerEquipButtonPressed();
		}
	}
}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ABlasterCharacter::CrouchButtonPressed(const FInputActionValue& Value)
{
	 if(bIsCrouched)
	 {
		 UnCrouch();
	 }
	 else
	 {
	 	Crouch();
	 }
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	// This is for the server scenario, since RepNotify functions are not called on the server
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	
	OverlappingWeapon = Weapon;
	
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	// This is for the client scenario, since RepNotify functions are only called on clients
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

bool ABlasterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}




