#pragma once
#include "../../CoreMinimal.h"

class FortGameModeAthena
{
public:
	static bool ReadyToStartMatch(AFortGameModeAthena* __this);
	static APawn* SpawnDefaultPawnFor(AFortGameModeAthena* __this, AController* NewPlayer, AActor* StartSpot);
	static void CreateAIDirector(AFortGameModeZone* __this);
	static void Setup();
};