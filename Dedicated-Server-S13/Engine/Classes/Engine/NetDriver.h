#pragma once
#include "../../CoreMinimal.h"

class NetDriver
{
private:
	class Originals
	{
	public:
		static inline void (*TickFlush)(UNetDriver*);
	};
public:
	static void TickFlush(UNetDriver* __this);
	static void Setup();
};