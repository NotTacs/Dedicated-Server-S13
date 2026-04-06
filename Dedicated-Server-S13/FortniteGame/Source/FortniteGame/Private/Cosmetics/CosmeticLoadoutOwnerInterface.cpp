#include "pch.h"
#include "../../Public/Cosmetics/CosmeticLoadoutOwnerInterface.h"

void CosmeticLoadoutOwnerInterface::SetCosmeticLoadout(ICosmeticLoadoutOwner* __this, FFortAthenaLoadout* CosmeticLoadout)
{
	printf(__FUNCTION__"\n");
	UObject* Controller = __this->GetOwner();
	printf("Controller: %s\n", Controller->GetName().c_str());
	if (AFortAthenaAIBotController* AIController = Cast<AFortAthenaAIBotController>(Controller))
	{
		CosmeticLoadout = &AIController->BotData->CharacterCustomization->CustomizationLoadout;
		AIController->CosmeticLoadoutBC = *CosmeticLoadout;
	}
}

void CosmeticLoadoutOwnerInterface::Setup()
{
	UFortHookingLibrary::Get()->Hook(AFortAthenaAIBotController::GetDefaultObj()->GetInterfaceAddress<ICosmeticLoadoutOwner>()->VTable, 0x4, CosmeticLoadoutOwnerInterface::SetCosmeticLoadout);
}