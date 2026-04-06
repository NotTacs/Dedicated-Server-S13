#pragma once
#include "../../CoreMinimal.h"

class GameSession
{
public:
	static bool KickPlayer(AGameSession* __this, APlayerController* Player, const FText& Reason);
	static void Setup();
};