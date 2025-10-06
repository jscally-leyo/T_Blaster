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
	
	// Input Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);

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
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleDefaultsOnly)
	UCombatComponent* Combat;
	
public:
	void SetOverlappingWeapon(AWeapon* Weapon);
};
