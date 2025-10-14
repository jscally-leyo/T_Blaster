// All rights preserved to Leyodemus

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class BLASTER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar; // Name must be the exact same in the WBP
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText; // Name must be the exact same in the WBP
};
