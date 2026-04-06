#include "pch.h"
#include "../../../Public/Athena/AI/FortAthenaAIBotCustomizationData.h"

void FortAthenaAIBotCustomizationData::FetchCustomizationLoadout(UFortAthenaAIBotCustomizationData* __this, AFortPlayerPawnAthena* Pawn, FFortAthenaLoadout* OutLoadout, FFortAthenaAIBotRunTimeCustomizationData* RuntimeData)
{
	UFortAthenaAIBotCosmeticLibraryData* CustomizationLibrary = StaticLoadObject<UFortAthenaAIBotCosmeticLibraryData>(
		UKismetStringLibrary::Conv_NameToString(__this->CosmeticCustomizationLibrary.ObjectID.AssetPathName).ToString());
	printf("CustomizationLibrary: %s\n", CustomizationLibrary->GetName().c_str());
	UDataTable* PredefineSetsDataTable = StaticLoadObject<UDataTable>(
		UKismetStringLibrary::Conv_NameToString(CustomizationLibrary->PredefineSetsDataTable.ObjectID.AssetPathName).ToString());
	if (PredefineSetsDataTable)
	{
		printf("PrefinedCosmeticSetTag: %s\n", RuntimeData->PredefinedCosmeticSetTag.TagName.ToString().c_str());
		TArray<FFortBotCosmeticItemSetDataTableRow*> TableRow;
		for (auto& [Key, Value] : PredefineSetsDataTable->RowMap)
		{
			FFortBotCosmeticItemSetDataTableRow* Data = reinterpret_cast<FFortBotCosmeticItemSetDataTableRow*>(Value);
			if (!Data)
				continue;
			if (__this->CharacterCustomization->CustomizationLoadout.Character->GetName() != "CID_556_Athena_Commando_F_RebirthDefaultA")
				continue;
			if (Data->SetTag.TagName == RuntimeData->PredefinedCosmeticSetTag.TagName)
			{
				UAthenaCharacterItemDefinition* CharacterItemDefinition = Cast<UAthenaCharacterItemDefinition>(UKismetSystemLibrary::GetObjectFromPrimaryAssetId(Data->CharacterAssetId));
				__this->CharacterCustomization->CustomizationLoadout.Character = CharacterItemDefinition;
				__this->CharacterCustomization->CustomizationLoadout.Glider = Cast<UAthenaGliderItemDefinition>(UKismetSystemLibrary::GetObjectFromPrimaryAssetId(Data->GliderAssetId));
				__this->CharacterCustomization->CustomizationLoadout.SkyDiveContrail = Cast<UAthenaSkyDiveContrailItemDefinition>(UKismetSystemLibrary::GetObjectFromPrimaryAssetId(Data->SkyDiveContrailAssetId));
				__this->CharacterCustomization->CustomizationLoadout.Backpack = Cast<UAthenaBackpackItemDefinition>(UKismetSystemLibrary::GetObjectFromPrimaryAssetId(Data->BackpackAssetId));
				__this->CharacterCustomization->CustomizationLoadout.Pickaxe = Cast<UAthenaPickaxeItemDefinition>(UKismetSystemLibrary::GetObjectFromPrimaryAssetId(Data->PickaxeAssetId));
			}
		}
	}

	*OutLoadout = __this->CharacterCustomization->CustomizationLoadout;
}

void FortAthenaAIBotCustomizationData::Setup()
{
	UFortHookingLibrary::Get()->Rel32Swap_Ptr(0x1E5699E, FetchCustomizationLoadout);
}