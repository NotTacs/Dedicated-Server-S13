#pragma once
#include "../../CoreMinimal.h"

class FortAthenaAIBotCustomizationData
{
public:
	static void FetchCustomizationLoadout(UFortAthenaAIBotCustomizationData* __this, AFortPlayerPawnAthena* Pawn, FFortAthenaLoadout* OutLoadout, FFortAthenaAIBotRunTimeCustomizationData* RuntimeData);
	static void Setup();
};