#include "pch.h"
#include "../../Public/Athena/FortGameModeAthena.h"
#include "../../Public/Athena/FortAthenaLivingWorldManager.h"

void __fastcall InitializePlaylistDataPreDataLoad(AFortGameStateAthena* __this, double _XMM1_8)
{
	UFortPlaylistAthena* OverridePlaylist;

	OverridePlaylist = __this->CurrentPlaylistInfo.OverridePlaylist;
	if (OverridePlaylist || (OverridePlaylist = __this->CurrentPlaylistInfo.BasePlaylist) != 0)
	{
		if (__this->Role == ENetRole::ROLE_Authority)
			__this->TeamCount = OverridePlaylist->MaxTeamCount;
		float FinalWinConditionTime = UFortScalableFloatUtils::GetValueAtLevel(OverridePlaylist->FinalWinConditionTime, 0);
		__this->TotalFinalCountdownTime = FinalWinConditionTime;
	}
}

bool FortGameModeAthena::ReadyToStartMatch(AFortGameModeAthena* __this)
{
	AFortGameStateAthena* GameState = Cast<AFortGameStateAthena>(__this->GameState);
	if (!GameState)
		return false;

	if (__this->CurrentPlaylistId == -1)
	{
		UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultSolo.Playlist_DefaultSolo");
		GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
		GameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
		GameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
		GameState->CurrentPlaylistInfo.MarkArrayDirty();
		GameState->CurrentPlaylistId = Playlist->PlaylistId;
		__this->CurrentPlaylistName = Playlist->PlaylistName;

		__this->CurrentPlaylistId = Playlist->PlaylistId;

		__this->WarmupRequiredPlayerCount = 1;

		
	}

	if (!GameState->MapInfo)
		return false;

	static bool bSetReady = false;
	if (!bSetReady)
	{
		printf("YES\n");

		auto Playlist = GameState->CurrentPlaylistInfo.BasePlaylist;

		UCurveTable* AthenaGameData = StaticLoadObject<UCurveTable>(UKismetStringLibrary::Conv_NameToString(Playlist->GameData.ObjectID.AssetPathName).ToString());
		for (auto& [Key, Value] : AthenaGameData->GetRowMap<FSimpleCurve*>())
		{
			if (Key.ToString() == "Default.Lager.Enable" || Key.ToString() == "Default.Phoebe.Enable" || Key.ToString() == "Default.Mang.Enable")
			{
				printf("Yeah");
				Value->Keys[0].Value = 1;
			}
		}



		GameState->AirCraftBehavior = Playlist->AirCraftBehavior;
		if (Playlist->AISettings)
		{
			__this->AISettings = Playlist->AISettings;
		}

		for (auto& AdditionalServerLevel : Playlist->AdditionalLevelsServerOnly)
		{
			UWorld* World = StaticLoadObject<UWorld>(UKismetStringLibrary::Conv_NameToString(AdditionalServerLevel.ObjectID.AssetPathName).ToString());
			if (World)
			{
				if (World->PersistentLevel && World->PersistentLevel->Actors.Num() > 0)
				{
					for (auto& Actor : World->PersistentLevel->Actors)
					{
						if (Actor && Actor->IsA(ABuildingFoundation::StaticClass()))
						{
							printf("Actor Found.\n");
							ABuildingFoundation* Foundation = Cast<ABuildingFoundation>(Actor);
							for (auto& AdditionalWorld : Foundation->AdditionalWorlds)
							{
								ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GWorld.Fetch(), AdditionalWorld, {}, {}, nullptr, {});
								FAdditionalLevelStreamed LevelStreamed{};
								LevelStreamed.bIsServerOnly = true;
								LevelStreamed.LevelName = AdditionalWorld.ObjectID.AssetPathName;
								GameState->AdditionalPlaylistLevelsStreamed.Add(LevelStreamed);

							}
						}
					}
				}
			}
			ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GWorld.Fetch(), AdditionalServerLevel, {}, {}, nullptr, {});
			FAdditionalLevelStreamed LevelStreamed{};
			LevelStreamed.bIsServerOnly = true;
			LevelStreamed.LevelName = AdditionalServerLevel.ObjectID.AssetPathName;
			GameState->AdditionalPlaylistLevelsStreamed.Add(LevelStreamed);
		}
		GameState->OnRep_AdditionalPlaylistLevelsStreamed();

		InitializePlaylistDataPreDataLoad(GameState, 0);
		static void (*LoadPlaylistData)(AFortGameStateAthena*) = decltype(LoadPlaylistData)(InSDKUtils::GetImageBase() + 0x1D8FA90);
		LoadPlaylistData(GameState);

		if (!__this->ServerBotManager)
		{
			__this->ServerBotManager = NewObject<UFortServerBotManagerAthena>(__this);
			__this->ServerBotManager->CachedGameState = GameState;
			__this->ServerBotManager->CachedGameMode = __this;
			*reinterpret_cast<bool*>(__int64(__this->ServerBotManager) + 0x4D0) = true;
		}
		if (!__this->SpawningPolicyManager)
		{
			__this->SpawningPolicyManager = NewObject<AFortAthenaSpawningPolicyManager>(__this);
			__this->SpawningPolicyManager->GameStateAthena = GameState;
			__this->SpawningPolicyManager->GameModeAthena = __this;
		}

		UWorld::GetWorld()->AISystem->PlayerBotManager = __this->ServerBotManager;

		__this->bWorldIsReady = true;

		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(UWorld::GetWorld(), AAthenaNavInvokerBox::StaticClass(), &Actors);
		for (AActor* Actor : Actors)
		{
			AAthenaNavInvokerBox* NavInvokerBox = Cast<AAthenaNavInvokerBox>(Actor);
			NavInvokerBox->GenerationRange = 5000;
		}

		bSetReady = true;
	}

	return __this->AlivePlayers.Num() > 0;
}

APawn* FortGameModeAthena::SpawnDefaultPawnFor(AFortGameModeAthena* __this, AController* NewPlayer, AActor* StartSpot)
{
	printf(__FUNCTION__"\n");
	return __this->SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetTransform());
}

void FortGameModeAthena::CreateAIDirector(AFortGameModeZone* __this)
{
	__this->AIDirector = SpawnActor<AAthenaAIDirector>();
}

void FortGameModeAthena::Setup()
{
	UFortHookingLibrary::Get()->Hook(0x1D53950, ReadyToStartMatch);
	UFortHookingLibrary::Get()->Hook(AFortGameModeAthena::GetDefaultObj()->VTable, 0xC8, SpawnDefaultPawnFor);
	UFortHookingLibrary::Get()->Rel32Swap_Ptr(0x1D311CB, CreateAIDirector);
}

