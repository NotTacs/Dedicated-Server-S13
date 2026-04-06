#include "pch.h"
#include "../../Public/Athena/FortAthenaLivingWorldManager.h"

void FortAthenaLivingWorldManager::LivingWorldManagerRegisterPointProvider(AActor* PointProvider)
{
	printf(__FUNCTION__"\n");
	UFortAthenaLivingWorldManager* LivingWorldManager = GetCurrent(UWorld::GetWorld());
	if (!LivingWorldManager)
		return;
	LivingWorldManager->PointProviders.Add(PointProvider);
}

void FortAthenaLivingWorldManager::LivingWorldManagerUnregisterPointProvider(AActor* ActorProvider)
{
	printf(__FUNCTION__"\n");
	UFortAthenaLivingWorldManager* LivingWorldManager = GetCurrent(UWorld::GetWorld());
	if (!LivingWorldManager)
		return;
	int32 Index = LivingWorldManager->PointProviders.Find(ActorProvider);
	if (Index == -1)
		return;
	LivingWorldManager->PointProviders.Remove(Index);
}

UFortAthenaLivingWorldManager* FortAthenaLivingWorldManager::GetCurrent(UObject* WorldContextObject)
{
	AFortGameStateAthena* GameState = Cast<AFortGameStateAthena>(UWorld::GetWorld()->GameState);
	UFortAthenaLivingWorldManager* LivingWorldManager = (UFortAthenaLivingWorldManager*)GameState->GetComponentByClass(UFortAthenaLivingWorldManager::StaticClass());
	return LivingWorldManager;
}

void FortAthenaLivingWorldManager::LivingWorldManagerGenerateEvents(UFortAthenaLivingWorldManager* __this, FFrame* Stack, void* Ret)
{
	Stack->Code() += Stack->Code() != 0;
	printf(__FUNCTION__"\n");
}

void FortAthenaLivingWorldManager::Setup()
{
	UFortHookingLibrary::Get()->Hook(0xD45260, LivingWorldManagerRegisterPointProvider);
	UFortHookingLibrary::Get()->Hook(0xD45270, LivingWorldManagerUnregisterPointProvider);
	UFortHookingLibrary::Get()->ExecHook(UObject::FindObjectFast<UFunction>("LivingWorldManagerGenerateEvents"), LivingWorldManagerGenerateEvents);

}