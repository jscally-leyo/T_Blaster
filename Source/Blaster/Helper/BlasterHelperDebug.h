#pragma once

namespace BlasterHelperDebug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::Orange, int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey,10.f, Color, Msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
}
