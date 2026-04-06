#pragma once
#include "Minhook/MinHook.h"
#include <type_traits>
#include <iostream>

class UFortHookingLibrary
{
public:
	static UFortHookingLibrary* Get()
	{
		static UFortHookingLibrary* Instance = new UFortHookingLibrary();
		return Instance;
	}

	void Initialize()
	{
		while (MH_Initialize() != MH_STATUS::MH_OK)
		{
			Sleep(100);
		}
	}

	static inline void* nullptrForHook = nullptr;

	template<typename T = void*>
	void Hook(uintptr_t Target, void* Detour, T& Original = nullptrForHook)
	{
		Target = Target += InSDKUtils::GetImageBase();
		while (MH_CreateHook(reinterpret_cast<void*>(Target), Detour, reinterpret_cast<void**>(&Original)) != MH_STATUS::MH_OK)
		{
			Sleep(100);
		}
		MH_EnableHook(reinterpret_cast<void*>(Target));
	}

	template<typename T>
	void Patch(uintptr_t Address, T Value)
	{
		DWORD og;
		VirtualProtect(LPVOID(Address), sizeof(T), PAGE_EXECUTE_READWRITE, &og);
		*(T*)Address = Value;
		VirtualProtect(LPVOID(Address), sizeof(T), og, &og);
	}

	static FORCEINLINE void ExecHook(UFunction* Func, void* Detour, void** OG = nullptr)
	{
		if (OG)
			*OG = Func->ExecFunction;
		Func->ExecFunction = reinterpret_cast<UFunction::FNativeFuncPtr>(Detour);
	}

	template<typename T = void*>
	void Hook(void** VTable, int Index, void* Detour, T& Original = nullptrForHook)
	{
		if (!std::is_same_v<T,void *>)
		{
			Original = (T)VTable[Index];
		}
		DWORD OldProtect;
		VirtualProtect(VTable + Index, sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtect);
		VTable[Index] = Detour;
		VirtualProtect(VTable + Index, sizeof(void*), OldProtect, &OldProtect);
	}

	static bool ReturnTrueHook()
	{
		return true;
	}

	static void EmptyHook()
	{
		return;
	}

private:
	uint8_t* AllocateNearbyPage(void* targetAddr)
	{
		SYSTEM_INFO SysInfo;
		GetSystemInfo(&SysInfo);

		const uint64_t PageSize = SysInfo.dwPageSize;
		const uint64_t StartAddr = (uint64_t(targetAddr) & ~(PageSize - 1));
		const uint64_t MinAddr = min(StartAddr - 0x7FFFFF00, (uint64_t)SysInfo.lpMinimumApplicationAddress);
		const uint64_t MaxAddr = max(StartAddr + 0x7FFFFF00, (uint64_t)SysInfo.lpMaximumApplicationAddress);
		const uint64_t StartPage = (StartAddr - (StartAddr % PageSize));

		for (uint64_t PageOffset = 1; PageOffset; PageOffset++)
		{
			uint64_t ByteOffset = PageOffset * PageSize;
			uint64_t HighAddr = StartPage + ByteOffset;
			uint64_t LowAddr = (StartPage > ByteOffset) ? StartPage - ByteOffset : 0;

			bool NeedsExit = HighAddr > MaxAddr && LowAddr < MinAddr;

			if (HighAddr < MaxAddr)
			{
				if (void* OutAddr = VirtualAlloc((void*)HighAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
				{
					return (uint8_t*)OutAddr;
				}
			}

			if (LowAddr > MinAddr)
			{
				if (void* OutAddr = VirtualAlloc((void*)LowAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
				{
					return (uint8_t*)OutAddr;
				}
			}

			if (NeedsExit)
			{
				break;
			}
		}

		return nullptr;
	}
public:
	void Rel32Swap_Ptr(uintptr_t Target, void* Detour)
	{
		Target = Target + InSDKUtils::GetImageBase();
		void** T = reinterpret_cast<void**>(&Target);
		auto Impl = (uint8*)(*T);
		auto NearPage = AllocateNearbyPage(Impl);

		if (!NearPage)
		{
			return;
		}

		uint8_t Shellcode[] =
		{
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp [$+6]
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		memcpy(Shellcode + 6, &Detour, 8);
		memcpy(NearPage, Shellcode, sizeof(Shellcode));

		auto Offset = static_cast<int>(NearPage - (Impl + 5));

		memcpy(Impl + 1, &Offset, sizeof(int));
	}
};