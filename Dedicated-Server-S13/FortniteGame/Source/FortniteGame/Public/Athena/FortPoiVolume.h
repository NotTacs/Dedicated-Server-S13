#pragma once
#include "../../CoreMinimal.h"

class FortPoiVolume
{
private:
	class Originals
	{
	public:
		static inline void (*PostInitializeComponents)(AFortPoiVolume* __this);
	};
public:
	static void PostInitializeComponents(AFortPoiVolume* __this);
	static void Setup();
};