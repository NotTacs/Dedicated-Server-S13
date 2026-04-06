#include "pch.h"
#include "../../Public/Building/BuildingFoundation.h"

void BuildingFoundation::SetDynamicBuildingFoundationEnabled(ABuildingFoundation* __this, bool bNewEnabled)
{
	EDynamicFoundationEnabledState NewState = bNewEnabled ? EDynamicFoundationEnabledState::Enabled : EDynamicFoundationEnabledState::Disabled;

	__this->FoundationEnabledState = NewState;
	__this->DynamicFoundationRepData.EnabledState = NewState;
	__this->OnRep_DynamicFoundationRepData();
}

void BuildingFoundation::SetDynamicFoundationTransform(ABuildingFoundation* __this, const FTransform& NewTransform)
{
	__this->DynamicFoundationTransform = NewTransform;
	__this->DynamicFoundationRepData.Translation = NewTransform.Translation;
	__this->DynamicFoundationRepData.Rotation = NewTransform.Rotation;
	__this->OnRep_DynamicFoundationRepData();
}

void BuildingFoundation::Setup()
{
	UFortHookingLibrary::Get()->Rel32Swap_Ptr(0x2B80A91, SetDynamicFoundationTransform);
	UFortHookingLibrary::Get()->Rel32Swap_Ptr(0x1511F0D, SetDynamicBuildingFoundationEnabled);
}