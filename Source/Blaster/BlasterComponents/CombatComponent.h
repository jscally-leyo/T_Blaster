// All rights preserved to Leyodemus

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ABlasterCharacter;
class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class ABlasterCharacter; // This gives the character class FULL access to everything from this component class (which is fine here, but use carefully)
	
	void EquipWeapon(AWeapon* WeaponToEquip);
	
protected:
	virtual void BeginPlay() override;

private:
	ABlasterCharacter* Character;
	AWeapon* EquippedWeapon;

public:
	
};
