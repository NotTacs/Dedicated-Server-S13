#include "pch.h"
#include "../../FortniteGame/Source/FortniteGame/Public/Athena/FortPlayerControllerAthena.h"

void FortPlayerControllerAthena::ServerAcknowledgePossession(AFortPlayerControllerAthena* __this, APawn* P)
{
	__this->AcknowledgedPawn = P;
    if (!__this->MyFortPawn)
        return;
    AFortPlayerPawn* PlayerPawn = __this->MyFortPawn;
    PlayerPawn->ResetGameplayAttributes(false);
    static void (*UFortGlobals__InitializeGameplayAbilities)(IAbilitySystemInterface*) = decltype(UFortGlobals__InitializeGameplayAbilities)(InSDKUtils::GetImageBase() + 0x2328070);
    UFortGlobals__InitializeGameplayAbilities(__this->PlayerState->GetInterfaceAddress<IAbilitySystemInterface>());

    static void (*AFortGameStateZone__OnAbilitySystemInterfaceReadyForPersistentGameplayEffects)(AFortGameStateZone * __this, IAbilitySystemInterface * InInterface) = decltype(AFortGameStateZone__OnAbilitySystemInterfaceReadyForPersistentGameplayEffects)(InSDKUtils::GetImageBase() + 0x2329D50);
    AFortGameStateZone__OnAbilitySystemInterfaceReadyForPersistentGameplayEffects(UWorld::GetWorld()->GameState, PlayerPawn->GetInterfaceAddress<IAbilitySystemInterface>());
}

void FortPlayerControllerAthena::Setup()
{
    UFortHookingLibrary::Get()->Hook(AFortPlayerControllerAthena::GetDefaultObj()->VTable, (0x868 / 8), ServerAcknowledgePossession);
}