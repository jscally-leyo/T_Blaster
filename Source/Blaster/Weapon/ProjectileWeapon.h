// All rights preserved to Leyodemus

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

UCLASS()
class BLASTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AProjectileWeapon();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
};
