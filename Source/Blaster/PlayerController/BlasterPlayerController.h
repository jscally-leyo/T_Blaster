// All rights preserved to Leyodemus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

class ABlasterHUD;

UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score); // Float because the built-in Score variable on the Player State is a float as well
	void SetHUDDefeats(int32 Defeats); // There is not Defeats variable on the Player State, so we might as well take an integer here
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY() // It's always good to add this UPROPERTY() macro because then the variable is automatically initialized as a nullptr
	ABlasterHUD* BlasterHUD;
};
