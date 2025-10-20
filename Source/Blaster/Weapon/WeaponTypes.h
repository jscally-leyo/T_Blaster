// All rights preserved to Leyodemus

#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	
	EWT_MAX UMETA(DisplayName = "DefaultMAX") // Not required, but easy to use for counting the number of options we have in an enum
};