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

	return Originals::TickFlush(__this);
}

void NetDriver::Setup()
{
	UFortHookingLibrary::Get()->Hook(0x49494B0, TickFlush, Originals::TickFlush);
}
