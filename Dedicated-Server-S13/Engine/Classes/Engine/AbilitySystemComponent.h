#pragma once
#include "../../CoreMinimal.h"

class AbilitySystemComponent
{
public:
	static void ConsumeAllReplicatedData(UAbilitySystemComponent* Component, FGameplayAbilitySpecHandle AbilityHandle, const FPredictionKey* AbilityOriginalPredictionKey);
	static void InternalServerTryActivateAbility(UAbilitySystemComponent* Component, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData);
	static void Setup();
};