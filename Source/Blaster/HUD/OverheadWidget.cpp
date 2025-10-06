// All rights preserved to Leyodemus

#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	// There is a 5-second delay when this function in called in the CharacterBP-class, to make sure everything (GameState, ...) is loaded before text is fetched
	
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;

	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ROLE_None:
		Role = FString("None");
		break;
	}

	FString PlayerNameFromPlayerState = "NotFound";
	APlayerState* PlayerState = InPawn->GetPlayerState();
	if (PlayerState)
	{
		PlayerNameFromPlayerState = PlayerState->GetPlayerName();
	}
	
	FString RemoteRoleString = FString::Printf(TEXT("Name: %s \nRemote Role: %s"), *PlayerNameFromPlayerState, *Role);

	SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
