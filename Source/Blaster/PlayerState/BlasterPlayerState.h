// All rights preserved to Leyodemus

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlasterPlayerState.generated.h"

class ABlasterPlayerController;
class ABlasterCharacter;

UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	
	/**
	 * Replication notifies
	 */
	virtual void OnRep_Score() override;
	
	UFUNCTION()
	virtual void OnRep_Defeats();
	
	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);
	
private:
	UPROPERTY() // It's always good to add this UPROPERTY() macro because then the variable is automatically initialized as a nullptr
	ABlasterCharacter* Character;
	UPROPERTY() // It's always good to add this UPROPERTY() macro because then the variable is automatically initialized as a nullptr
	ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;
	
};
