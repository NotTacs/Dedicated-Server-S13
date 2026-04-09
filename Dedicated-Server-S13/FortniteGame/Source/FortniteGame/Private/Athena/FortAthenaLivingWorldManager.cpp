#include "pch.h"
#include "../../Public/Athena/FortAthenaLivingWorldManager.h"

void FortAthenaLivingWorldManager::LivingWorldManagerRegisterPointProvider(AActor* PointProvider)
{
	printf(__FUNCTION__"\n");
	UFortAthenaLivingWorldManager* LivingWorldManager = GetCurrent(UWorld::GetWorld());
	if (!LivingWorldManager)
		return;
	LivingWorldManager->PointProviders.Add(PointProvider);
}

void FortAthenaLivingWorldManager::LivingWorldManagerUnregisterPointProvider(AActor* ActorProvider)
{
	printf(__FUNCTION__"\n");
	UFortAthenaLivingWorldManager* LivingWorldManager = GetCurrent(UWorld::GetWorld());
	if (!LivingWorldManager)
		return;
	int32 Index = LivingWorldManager->PointProviders.Find(ActorProvider);
	if (Index == -1)
		return;
	LivingWorldManager->PointProviders.Remove(Index);
}

UFortAthenaLivingWorldManager* FortAthenaLivingWorldManager::GetCurrent(UObject* WorldContextObject)
{
	AFortGameStateAthena* GameState = Cast<AFortGameStateAthena>(UWorld::GetWorld()->GameState);
	UFortAthenaLivingWorldManager* LivingWorldManager = (UFortAthenaLivingWorldManager*)GameState->GetComponentByClass(UFortAthenaLivingWorldManager::StaticClass());
	return LivingWorldManager;
}

TArray<FFortAthenaLivingWorldCategory*> GetAllCategories(UFortAthenaLivingWorldManager* __this, const FString& ContextString)
{
	TArray<FFortAthenaLivingWorldCategory*> OutCategories;
	UFortAthenaLivingWorldConfigData* Config = __this->LagerConfig.Get();
	if (!Config)
	{
		Config = StaticLoadObject<UFortAthenaLivingWorldConfigData>(UKismetStringLibrary::Conv_NameToString(__this->LagerConfig.ObjectID.AssetPathName).ToString());
	}
	if (!Config)
	{
		UE_LOG(LogLivingWorldManager, Error, TEXT("Failed to fetch Config."));
		return OutCategories;
	}

	UDataTable* Table = Config->CategoryTable.Get();
	if (!Table)
	{
		Table = StaticLoadObject<UDataTable>(UKismetStringLibrary::Conv_NameToString(Config->CategoryTable.ObjectID.AssetPathName).ToString());
	}

	if (Table)
	{
		for (auto& [Key, Value] : Table->RowMap)
		{
			auto Category = (FFortAthenaLivingWorldCategory*)Value;
			OutCategories.Add(Category);
		}
	}

	return OutCategories;
}

const FFortAthenaLivingWorldCategory* FindFirstActiveCategory(UFortAthenaLivingWorldManager* __this, const FFortAthenaLivingWorldEvent* Event)
{
	if (!Event)
		return nullptr;
	TArray<FFortAthenaLivingWorldCategory*> Categories;
	GetAllCategories(__this, TEXT("UFortAthenaLivingWorldManager::FindFirstActiveCategory"));

	const FFortAthenaLivingWorldCategory* FoundCategory = nullptr;

	for (const FFortAthenaLivingWorldCategory* Category : Categories)
	{
		if (!Category) continue;

		if (Category->RequirePlaylistTags == UWorld::GetWorld()->GameState->CurrentPlaylistInfo.BasePlaylist->GameplayTagContainer)
		{
			UDataTable* EventTable = StaticLoadObject<UDataTable>(UKismetStringLibrary::Conv_NameToString(Category->EventTable.ObjectID.AssetPathName).ToString());
			if (EventTable)
			{
				TArray<FFortAthenaLivingWorldEvent*> CategoryEvents;
				for (auto& [Key, Value] : EventTable->RowMap)
				{
					FFortAthenaLivingWorldEvent* CategoryEvent = (FFortAthenaLivingWorldEvent*)Value;
					if (CategoryEvent == Event)
					{
						FoundCategory = Category;
						break;
					}
				}
			}
		}
	}

	return FoundCategory;
}

const FFortAthenaLivingWorldCategory* FindFirstActiveCategory(UFortAthenaLivingWorldManager* __this, const UDataTable* EventTable)
{
	if (!EventTable)
		return nullptr;
	TArray<FFortAthenaLivingWorldCategory*> Categories;
	GetAllCategories(__this, TEXT("UFortAthenaLivingWorldManager::FindFirstActiveCategory"));

	const FFortAthenaLivingWorldCategory* FoundCategory = nullptr;

	for (const FFortAthenaLivingWorldCategory* Category : Categories)
	{
		if (!Category) continue;

		if (Category->RequirePlaylistTags == UWorld::GetWorld()->GameState->CurrentPlaylistInfo.BasePlaylist->GameplayTagContainer)
		{
			UDataTable* LoadedTable = StaticLoadObject<UDataTable>(UKismetStringLibrary::Conv_NameToString(Category->EventTable.ObjectID.AssetPathName).ToString());
			if (LoadedTable == EventTable)
			{
				FoundCategory = Category;
				break;
			}
		}
	}

	return FoundCategory;
}

void FortAthenaLivingWorldManager::LivingWorldManagerGenerateEvents(UFortAthenaLivingWorldManager* __this, FFrame* Stack, void* Ret)
{
	Stack->Code() += Stack->Code() != 0;
	printf(__FUNCTION__"\n");
}

void FortAthenaLivingWorldManager::Setup()
{
	UFortHookingLibrary::Get()->Hook(0xD45260, LivingWorldManagerRegisterPointProvider);
	UFortHookingLibrary::Get()->Hook(0xD45270, LivingWorldManagerUnregisterPointProvider);
	UFortHookingLibrary::Get()->ExecHook(UObject::FindObjectFast<UFunction>("LivingWorldManagerGenerateEvents"), LivingWorldManagerGenerateEvents);
}