#pragma once
#include "../../CoreMinimal.h"

class FortAthenaLivingWorldManager
{
public:
	static void LivingWorldManagerRegisterPointProvider(AActor* PointProvider);
	static void LivingWorldManagerUnregisterPointProvider(AActor* ActorProvider);
	static void LivingWorldManagerGenerateEvents(UFortAthenaLivingWorldManager* __this, FFrame* Stack, void* Ret);
	static UFortAthenaLivingWorldManager* GetCurrent(UObject* WorldContextObject);
	static void Setup();
};