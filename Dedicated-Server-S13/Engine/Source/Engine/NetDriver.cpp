#include "pch.h"
#include "../../Classes/Engine/NetDriver.h"

void NetDriver::TickFlush(UNetDriver* __this)
{
	if (__this->ReplicationDriver)
	{
		static void (*ServerReplicateActors)(UReplicationDriver*) = decltype(ServerReplicateActors)(InSDKUtils::GetImageBase() + 0x140E720);
		ServerReplicateActors(__this->ReplicationDriver);
	}

	if (GetAsyncKeyState(VK_F8) & 1)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"startaircraft", nullptr);
	}

	if (GetAsyncKeyState(VK_F9) & 1)
	{
		AFortGameModeAthena* GameMode = Cast<AFortGameModeAthena>(UWorld::GetWorld()->AuthorityGameMode);
		UFortAthenaAISpawnerData* SpawnerData = StaticLoadObject<UFortAthenaAISpawnerData>("/SpicySake/AISpawnerData/Parent/AISpawnerData_SpicySake_Parent_BP.Default__AISpawnerData_SpicySake_Parent_BP_C");
		auto List = SpawnerData->CreateComponentListFromClass(SpawnerData->Class, UWorld::GetWorld());
		GameMode->ServerBotManager->SpawnAI(__this->ClientConnections[0]->PlayerController->Pawn->K2_GetActorLocation(), {}, List);
	}

	return Originals::TickFlush(__this);
}

void NetDriver::Setup()
{
	UFortHookingLibrary::Get()->Hook(0x49494B0, TickFlush, Originals::TickFlush);
}
