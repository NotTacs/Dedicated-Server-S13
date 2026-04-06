#pragma once
#include "../../CoreMinimal.h"

class FortControllerComponent_Aircraft
{
public:
	static void ServerAttemptAircraftJump(UFortControllerComponent_Aircraft* __this, const FRotator& ClientRotation);
	static void Setup();
};