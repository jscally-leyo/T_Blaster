// All rights preserved to Leyodemus

#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

#include "Blaster/Helper/BlasterHelperDebug.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	
	if (NumberOfPlayers == 1)
	{
		BlasterHelperDebug::Print(TEXT("Session started, you are the first player!"));
	};
	
	if (NumberOfPlayers == 2)
	{
		if (UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
