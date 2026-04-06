#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <SDK.hpp>
using namespace SDK;
#include <FortHookingLibrary.h>

#define WITH_SERVER_CODE 1

struct FCommandLine
{
private:
	class Originals
	{
	public:
		using GetFn = const wchar_t* (*)();

		static inline GetFn Get = reinterpret_cast<GetFn>(InSDKUtils::GetImageBase() + 0x30EAC20);
	};
public:
	static const wchar_t* Get()
	{
		return Originals::Get();
	}
};

struct FParse
{
public:
	static bool Param(const TCHAR* Stream, const TCHAR* Param)
	{
		static bool (*ParamOriginal)(const TCHAR*, const TCHAR*) = decltype(ParamOriginal)(InSDKUtils::GetImageBase() + 0x311F420);
		return ParamOriginal(Stream, Param);
	}
};

inline void DedicatedLogEx(const wchar_t* category, const wchar_t* verbosity, const wchar_t* format, ...)
{
    va_list args;

    wprintf(L"[%s][%s] ", category, verbosity);
    va_start(args, format);
    vwprintf(format, args);
    va_end(args);
    wprintf(L"\n");

    FILE* file = nullptr;
    _wfopen_s(&file, L"DedicatedServer.log", L"a, ccs=UTF-8");
    if (file)
    {
        fwprintf(file, L"[%s][%s] ", category, verbosity);

        va_start(args, format);
        vfwprintf(file, format, args);
        va_end(args);

        fwprintf(file, L"\n");
        fclose(file);
    }
}

#define UE_LOG(Category, Verbosity, Format, ...) \
    DedicatedLogEx(L#Category, L#Verbosity, Format, ##__VA_ARGS__)

template<typename T>
T* Cast(UObject* Object)
{
    if (!Object)
        return nullptr;
    if (Object->IsA(T::StaticClass()))
		return static_cast<T*>(Object);
    return nullptr;
}

template<typename T>
T* NewObject(UObject* Outer)
{
    return (T*)UGameplayStatics::SpawnObject(T::StaticClass(), Outer);
}

inline UObject* (*LoadObject)(UClass*, UObject*, const TCHAR*, const TCHAR*, uint32_t, UObject*, bool) = decltype(LoadObject)(InSDKUtils::GetImageBase() + 0x33698D0);

template<typename T>
inline T* StaticLoadObject(std::wstring Path, UClass* InClass = T::StaticClass(), UObject* Outer = nullptr)
{
    return (T*)LoadObject(InClass, Outer, Path.c_str(), nullptr, 0, nullptr, false);
}

template<typename T>
inline T* StaticLoadObject(std::string Path, UClass* InClass = T::StaticClass(), UObject* Outer = nullptr)
{
    return (T*)LoadObject(InClass, Outer, std::wstring(Path.begin(), Path.end()).c_str(), nullptr, 0, nullptr, false);
}

class FFrame
{
public:
	unsigned __int8* MostRecentPropertyAddress() {
		return *reinterpret_cast<unsigned __int8**>(uint64_t(this) + 0x38);
	}

	unsigned __int8*& Code() {
		return *reinterpret_cast<unsigned __int8**>(uint64_t(this) + 0x20);
	}
	SDK::FProperty*& PropertyChainForCompiledIn() {
		return *reinterpret_cast<FProperty**>(uint64_t(this) + 0x80);
	}
	SDK::UObject* Object() {
		return *reinterpret_cast<UObject**>(uint64_t(this) + 0x18);
	}


public:


	inline void Step(SDK::UObject* Context, void* const RESULT_PARAM) {
		static void (*StepOriginal)(__int64, SDK::UObject*, void* const RESULT_PARAM) = decltype(StepOriginal)(InSDKUtils::GetImageBase() + 0x3AC1E30);
		StepOriginal(__int64(this), Context, RESULT_PARAM);
	}
	inline void StepExplicitProperty(void* Result, SDK::FProperty* Property) {
		static void (*StepExplicitPropertyOriginal)(__int64, void*, FProperty*) = decltype(StepExplicitPropertyOriginal)(InSDKUtils::GetImageBase() + 0x3AC1E60);
		StepExplicitPropertyOriginal(__int64(this), Result, Property);
	}


	inline void StepCompiledIn(void* const Result)
	{
		if (Code())
		{
			Step(Object(), Result);
		}
		else {
			FProperty* Prop = PropertyChainForCompiledIn();
			PropertyChainForCompiledIn() = (FProperty*)Prop->Next;
			StepExplicitProperty(Result, Prop);
		}
	}
};

template<typename T>
inline T* SpawnActor(FTransform Transform = {}, AActor* Owner = nullptr, UClass* OverrideClass = T::StaticClass())
{
	AActor* Start = UGameplayStatics::BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), OverrideClass, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, Owner);
	return (T*)UGameplayStatics::FinishSpawningActor(Start, Transform);
}