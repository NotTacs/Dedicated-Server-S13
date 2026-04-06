#include "pch.h"
#include "../../../Public/AI/Navigation/AthenaNavSystem.h"

void AthenaNavSystemConfig::CreateAndConfigureNavSystem(UAthenaNavSystemConfig* __this, UWorld* World)
{
	printf(__FUNCTION__"\n");
	__this->bAutoSpawnMissingNavData = true;
	__this->bAllowAutoRebuild = true;
	__this->bPrioritizeNavigationAroundSpawners = true;
	__this->bSupportRuntimeNavmeshDisabling = false;
	__this->bCreateOnClient = true;

	return Originals::CreateAndConfigureNavSystem(__this, World);
}

void AthenaNavSystemConfig::Setup()
{
	UFortHookingLibrary::Get()->Hook(0x1B16740, CreateAndConfigureNavSystem, Originals::CreateAndConfigureNavSystem);
}