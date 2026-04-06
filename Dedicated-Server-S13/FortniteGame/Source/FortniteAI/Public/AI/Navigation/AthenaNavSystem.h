#pragma once
#include "../../CoreMinimal.h"

class AthenaNavSystem
{
public:
	static void Setup();
};

class AthenaNavSystemConfig
{
private:
	class Originals
	{
	public:
		static inline void (*CreateAndConfigureNavSystem)(UAthenaNavSystemConfig*, UWorld*);
	};
public:
	static void CreateAndConfigureNavSystem(UAthenaNavSystemConfig* __this, UWorld* World);
	static void Setup();
};