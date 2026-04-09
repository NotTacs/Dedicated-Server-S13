#include "pch.h"
#include "Engine/Classes/Engine/World.h"
#include "FortniteGame/Source/FortniteGame/Public/Athena/FortGameModeAthena.h"
#include "Engine/Classes/Engine/GameSession.h"
#include "Engine/Classes/Engine/NetDriver.h"
#include "FortniteGame/Source/FortniteGame/Public/Athena/FortPlayerControllerAthena.h"
#include "Engine/Classes/Engine/AbilitySystemComponent.h"
#include "FortniteGame/Source/FortniteGame/Public/Building/BuildingFoundation.h"
#include "FortniteGame/Source/FortniteAI/Public/AI/Navigation/AthenaNavSystem.h"
#include "FortniteGame/Source/FortniteGame/Public/Components/FortControllerComponent_Aircraft.h"
#include "FortniteGame/Source/FortniteGame/Public/Athena/FortAthenaLivingWorldManager.h"
#include "FortniteGame/Source/FortniteGame/Public/Cosmetics/CosmeticLoadoutOwnerInterface.h"
#include "FortniteGame/Source/FortniteAI/Public/Athena/AI/FortAthenaAIBotCustomizationData.h"
#include "FortniteGame/Source/FortniteGame/Public/Athena/FortPoiVolume.h"

DWORD WINAPI Main(LPVOID lpThread)
{
    AllocConsole();
    FILE* File = nullptr;
	freopen_s(&File, "CONOUT$", "w+", stdout);
    UFortHookingLibrary::Get()->Initialize();
    Sleep(5000);
    *reinterpret_cast<bool*>(InSDKUtils::GetImageBase() + 0x899BCE8) = false;
	*reinterpret_cast<bool*>(InSDKUtils::GetImageBase() + 0x899BCE9) = true;
    World::Setup();
    FortGameModeAthena::Setup();
    GameSession::Setup();
    NetDriver::Setup();
    FortPlayerControllerAthena::Setup();
    AbilitySystemComponent::Setup();
    BuildingFoundation::Setup();
    FortControllerComponent_Aircraft::Setup();
    AthenaNavSystemConfig::Setup();
    FortAthenaLivingWorldManager::Setup();
    CosmeticLoadoutOwnerInterface::Setup();
    FortAthenaAIBotCustomizationData::Setup();
    FortPoiVolume::Setup();
    UFortHookingLibrary::Get()->Hook(0x3303E30, UFortHookingLibrary::ReturnTrueHook); //CanCreateInCurrentContext
    UFortHookingLibrary::Get()->Hook(0x22FF0B0, UFortHookingLibrary::EmptyHook); //ChangeGameSessionId

    UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogLivingWorldManager all", nullptr);
    UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"log LogAthenaBots all", nullptr);
    UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), L"open Apollo_Terrain", nullptr);
    UWorld::GetWorld()->OwningGameInstance->LocalPlayers.Remove(0);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == 1)
    {
        CreateThread(nullptr, 0, Main, nullptr, 0, nullptr);
    }
    return TRUE;
}

