#pragma once
#include "../../CoreMinimal.h"

class CosmeticLoadoutOwnerInterface
{
public:
	static void SetCosmeticLoadout(ICosmeticLoadoutOwner* __this, FFortAthenaLoadout* CosmeticLoadout);
	static void Setup();
};