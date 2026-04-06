#include "pch.h"
#include "../../Public/Cosmetics/CosmeticLoadoutOwnerInterface.h"

void CosmeticLoadoutOwnerInterface::SetCosmeticLoadout(ICosmeticLoadoutOwner* __this, FFortAthenaLoadout* CosmeticLoadout)
{
	printf(__FUNCTION__"\n");
	UObject* Controller = __this->GetOwner();
	printf("Controller: %s\n", Controller->GetName().c_str());
	if (AFortAthenaAIBotController* AIController = Cast<AFortAthenaAIBotController>(Controller))
	{
		printf("Character: %s\n", CosmeticLoadout->Character->GetName().c_str());
		AIController->CosmeticLoadoutBC = *CosmeticLoadout;

		static void (*ApplyCharacterCustomization)(void* PlayerState, void* Pawn) = decltype(ApplyCharacterCustomization)(InSDKUtils::GetImageBase() + 0x28455C0);
		ApplyCharacterCustomization(AIController->PlayerState, AIController->PlayerBotPawn);
	}
}

void CosmeticLoadoutOwnerInterface::Setup()
{
	UFortHookingLibrary::Get()->Hook(AFortAthenaAIBotController::GetDefaultObj()->GetInterfaceAddress<ICosmeticLoadoutOwner>()->VTable, 0x4, CosmeticLoadoutOwnerInterface::SetCosmeticLoadout);
}