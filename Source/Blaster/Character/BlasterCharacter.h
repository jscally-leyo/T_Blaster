// All rights preserved to Leyodemus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

class UCombatComponent;
class AWeapon;
class UWidgetComponent;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//!! built-in movement component is auto-replicated,
	//!! so moving, jumping, crouching and all variables (walk speed, crouch half height, ...) in the component will be automatically visible on all machines
	
	// IMPORTANT so that we can decide what has to be replicated
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void BeginPlay() override;

	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;
	
	// Input Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipButtonPressed(const FInputActionValue& Value);
	void CrouchButtonPressed(const FInputActionValue& Value);

	// Input variables
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	int CursorSpeed;

private:
	UPROPERTY(VisibleAnywhere, Category="Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* FollowCamera;
	
	UPROPERTY()
	APlayerController* PC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon) // See next function
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleDefaultsOnly)
	UCombatComponent* Combat;
	
	// RPC functions are one-way calls, called on the client to 'request' something from the server, or vice versa
	// For example: only server has authority so only server should decide if a character can pick up a weapon (this is also to prevent cheating!!)
	//              The server scenario for equipping is covered with the 'EquipButtonPressed' function, but for the client scenario...
	//              we need an RPC function that 'requests the equipping of the weapon for that client'
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
};
