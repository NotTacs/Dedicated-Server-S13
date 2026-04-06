#include "pch.h"
#include "../../Public/Components/FortControllerComponent_Aircraft.h"

void FortControllerComponent_Aircraft::ServerAttemptAircraftJump(UFortControllerComponent_Aircraft* __this, const FRotator& ClientRotation)
{
	AFortPlayerControllerAthena* PlayerControllerAthena = Cast<AFortPlayerControllerAthena>(__this->GetOwner());

	UWorld::GetWorld()->AuthorityGameMode->RestartPlayer(PlayerControllerAthena);
	PlayerControllerAthena->ClientSetRotation(ClientRotation, true);
}

void FortControllerComponent_Aircraft::Setup()
{
	UFortHookingLibrary::Get()->Hook(UFortControllerComponent_Aircraft::GetDefaultObj()->VTable, 0x89, ServerAttemptAircraftJump);
}