#pragma once
#include "../../CoreMinimal.h"

class FortAthenaLivingWorldManager
{
public:
	static void LivingWorldManagerRegisterPointProvider(AActor* PointProvider);
	static void LivingWorldManagerUnregisterPointProvider(AActor* ActorProvider);
	static void LivingWorldManagerGenerateEvents(UFortAthenaLivingWorldManager* __this, FFrame* Stack, void* Ret);
	static void OnWorldReady(UFortAthenaLivingWorldManager* __this);
	static UFortAthenaLivingWorldManager* GetCurrent(UObject* WorldContextObject);
	static void Setup();
};