#include "pch.h"
#include "../../Classes/Engine/World.h"

bool World::Listen(UWorld* __this, FURL& InURL)
{
	printf("World: %s\n", __this->GetName().c_str());
	FURL URL{};
	URL.Port = 7777;

#if WITH_SERVER_CODE
	if (__this->NetDriver)
	{
		GEngine->BroadcastNetworkFailure(__this, __this->NetDriver, ENetworkFailure::NetDriverAlreadyExists);
		return false;
	}

	// Create net driver.
	FName NetDriverDefinition = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");

	if (GEngine->CreateNamedNetDriver(__this, UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"), NetDriverDefinition))
	{
		__this->NetDriver = GEngine->FindNamedNetDriver(__this, UKismetStringLibrary::Conv_StringToName(L"GameNetDriver"));
		__this->NetDriver->NetDriverName = NetDriverDefinition;
		__this->NetDriver->World = __this;

		FLevelCollection* const SourceCollection = __this->FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->NetDriver = __this->NetDriver;
		}
		FLevelCollection* const StaticCollection = __this->FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			SourceCollection->NetDriver = __this->NetDriver;
		}
	}

	if (__this->NetDriver == nullptr)
	{
		GEngine->BroadcastNetworkFailure(__this, NULL, ENetworkFailure::NetDriverCreateFailure);
		return false;
	}

	AWorldSettings* WorldSettings = __this->K2_GetWorldSettings();
	const bool bReuseAddressAndPort = false;

	FString Error;
	if (!__this->NetDriver->InitListen(__this, URL, bReuseAddressAndPort, Error))
	{
		GEngine->BroadcastNetworkFailure(__this, __this->NetDriver, ENetworkFailure::NetDriverListenFailure, Error);
		UE_LOG(LogWorld, Log, TEXT("Failed to listen: %s"), *Error);
		//GEngine->DestroyNamedNetDriver(this, NetDriver->NetDriverName);
		__this->NetDriver = nullptr;
		FLevelCollection* SourceCollection = __this->FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->SetNetDriver(nullptr);
		}
		FLevelCollection* StaticCollection = __this->FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			StaticCollection->SetNetDriver(nullptr);
		}
		return false;
	}
	__this->NetDriver->SetWorld(__this);
	static const bool bLanPlay = FParse::Param(FCommandLine::Get(), TEXT("lanplay"));
	const bool bLanSpeed = bLanPlay || URL.HasOption(TEXT("LAN"));
	if (!bLanSpeed && (__this->NetDriver->MaxInternetClientRate < __this->NetDriver->MaxClientRate) && (__this->NetDriver->MaxInternetClientRate > 2500))
	{
		__this->NetDriver->MaxClientRate = __this->NetDriver->MaxInternetClientRate;
	}

	//__this->NextSwitchCountdown = __this->NetDriver->ServerTravelPause;
	return true;
#else
	return false;
#endif // WITH_SERVER_CODE
}

int World::GetNetMode(UWorld* __this)
{
	return 1; // NM_DedicatedServer
}

void World::Setup()
{
	UFortHookingLibrary::Get()->Rel32Swap_Ptr(0x4BD4A97, World::Listen);
	UFortHookingLibrary::Get()->Hook(0x4C247C0, World::GetNetMode);
}