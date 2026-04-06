#pragma once
#include "../../CoreMinimal.h"

class BuildingFoundation
{
public:
	static void SetDynamicBuildingFoundationEnabled(ABuildingFoundation* __this, bool bNewEnabled);
	static void SetDynamicFoundationTransform(ABuildingFoundation* __this, const FTransform& NewTransform);
	static void Setup();
};