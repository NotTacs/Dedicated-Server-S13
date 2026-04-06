#include "pch.h"
#include "../../Classes/Engine/AbilitySystemComponent.h"

static bool (*InternalTryActivateAbility)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, const FPredictionKey&, UGameplayAbility**,
	void*, const FGameplayEventData*) = decltype(InternalTryActivateAbility)(InSDKUtils::GetImageBase() + 0x999FE0);
static FGameplayAbilitySpec* (*FindAbilitySpecFromHandle)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle)
= decltype(FindAbilitySpecFromHandle)(InSDKUtils::GetImageBase() + 0x98BF60);

struct FAbilityReplicatedData
{
	bool bTriggered;
	FVector_NetQuantize100 VectorPayload;
	uint8 Delegate[0x18];
};

struct FAbilityReplicatedDataCache
{
	void** VTable;
	FGameplayAbilityTargetDataHandle TargetData;
	FGameplayTag ApplicationTag;
	bool bTargetConfirmed;
	bool bTargetCancelled;
	uint8 TargetSetDelegate[0x18];
	uint8 TargetCancelledDelegate[0x18];
	FAbilityReplicatedData GenericEvents[12];
	FPredictionKey PredictionKey;

public:
	void Reset()
	{
		static void (*ResetInternal)(FAbilityReplicatedDataCache*) = decltype(ResetInternal)(InSDKUtils::GetImageBase() + 0x9ADE40);
		ResetInternal(this);
	}
};

void AbilitySystemComponent::ConsumeAllReplicatedData(UAbilitySystemComponent* Component, FGameplayAbilitySpecHandle AbilityHandle,
	const FPredictionKey* AbilityOriginalPredictionKey)
{
	const int32 Current = AbilityOriginalPredictionKey->Current;
	struct FAbilityTargetDataEntry
	{
		int32 Handle;
		int32 Key;
		FAbilityReplicatedDataCache* Cache;
		void* ReferenceController;
	};
	auto* Entries = reinterpret_cast<FAbilityTargetDataEntry*>(Component->AbilityTargetDataMap.InUseData.GetDataPtr());
	const int32 ArrayNum = Component->AbilityTargetDataMap.InUseData.Num();

	FAbilityReplicatedDataCache* FoundCache = nullptr;

	for (int32 i = 0; i < ArrayNum; i++)
	{
		if (Entries[i].Handle == AbilityHandle.Handle &&
			Entries[i].Key == Current)
		{
			FoundCache = Entries[i].Cache;
			break;
		}
	}

	if (FoundCache)
	{
		FoundCache->Reset();
	}
}

void AbilitySystemComponent::InternalServerTryActivateAbility(UAbilitySystemComponent* Component, FGameplayAbilitySpecHandle Handle, bool InputPressed,
	const FPredictionKey& PredictionKey,
	const FGameplayEventData* TriggerEventData)
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(Component, Handle);
	if (!Spec)
	{
		Component->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	const UGameplayAbility* AbilityToActivate = Spec->Ability;

	if (!AbilityToActivate)
	{
		Component->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	/*if (AbilityToActivate->NetSecurityPolicy == EGameplayAbilityNetSecurityPolicy::ServerOnlyExecution
		|| AbilityToActivate->NetSecurityPolicy == EGameplayAbilityNetSecurityPolicy::ServerOnly)
	{
		Component->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}*/

	//ConsumeAllReplicatedData(Component, Handle, &PredictionKey);

	UGameplayAbility* InstancedAbility = nullptr;
	Spec->InputPressed = true;

	if (InternalTryActivateAbility(Component, Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
	{

	}
	else
	{
		Component->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		Spec->InputPressed = false;

		if (!Component->bCachedIsNetSimulated)
		{
			if (!(Spec->Ability && Spec->Ability->NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly && false))
			{
				Component->ActivatableAbilities.MarkItemDirty(*Spec);
			}
		}
		else
		{
			Component->ActivatableAbilities.MarkArrayDirty();
		}
	}
}

void AbilitySystemComponent::Setup()
{
	UFortHookingLibrary::Get()->Hook(UFortAbilitySystemComponentAthena::GetDefaultObj()->VTable, 0xFA, InternalServerTryActivateAbility);
	UFortHookingLibrary::Get()->Hook(UFortAbilitySystemComponent::GetDefaultObj()->VTable, 0xFA, InternalServerTryActivateAbility);
	UFortHookingLibrary::Get()->Hook(UAbilitySystemComponent::GetDefaultObj()->VTable, 0xFA, InternalServerTryActivateAbility);
}