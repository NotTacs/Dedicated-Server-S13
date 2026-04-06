#include "pch.h"
#include "../../Classes/Engine/GameSession.h"

bool GameSession::KickPlayer(AGameSession* __this, APlayerController* Player, const FText& Reason)
{
	printf("Kicking player %s for reason: %s\n", Player->Player->GetName().c_str(), Reason.ToString().c_str());
	return true;
}

void GameSession::Setup()
{
	UFortHookingLibrary::Get()->Hook(0x47E5060, KickPlayer);
}