// All rights preserved to Leyodemus

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "BlasterAnimInstance.generated.h"

class AWeapon;
class ABlasterCharacter;

UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	ABlasterCharacter* BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	AWeapon* EquippedWeapon;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bAiming;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float YawOffset;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Lean;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	// Aim offset
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float AO_Yaw; // Note: the _ will be filtered out in Blueprints
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float AO_Pitch; // Note: the _ will be filtered out in Blueprints

	// To fix left hand from moving away from the gun
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	ETurningInPlace TurningInPlace;

	// To fix right hand to point the gun at the crosshair target
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bElimmed;
};
