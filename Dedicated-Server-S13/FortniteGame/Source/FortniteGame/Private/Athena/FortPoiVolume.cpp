#include "pch.h"
#include "../../Public/Athena/FortPoiVolume.h"

static void (*RegisterComponent)(UActorComponent*) = decltype(RegisterComponent)(InSDKUtils::GetImageBase() + 0x4685b90);
static void (*SetGenerateOverlapEvents)(UBrushComponent*, bool) = decltype(SetGenerateOverlapEvents)(InSDKUtils::GetImageBase() + 0x46C12E0);
static void (*SetupAttachment)(USceneComponent*, UBrushComponent*, bool) = decltype(SetupAttachment)(InSDKUtils::GetImageBase() + 0x46E8B50);
void FortPoiVolume::PostInitializeComponents(AFortPoiVolume* __this)
{
	static TArray<ABuildingFoundation*> Foundations;
	UGameplayStatics::GetAllActorsOfClass(UWorld::GetWorld(), ABuildingFoundation::StaticClass(), (TArray<AActor*>*) & Foundations);
	FGameplayTagContainer& LocationTags = __this->LocationTags;
	for (auto& LocationTag : LocationTags.GameplayTags)
	{
		printf("LocationTag: %s\n", LocationTag.TagName.ToString().c_str());
	}

	ABuildingFoundation* FoundFoundation = nullptr;
	if (Foundations.Num() > 0)
	{
		for (auto& Foundation : Foundations)
		{
			static FName* (*GetBestPoiName)(ABuildingFoundation * __this, FName * result) = decltype(GetBestPoiName)(InSDKUtils::GetImageBase() + 0x1fe1900);
			FName BestPoiName;
			GetBestPoiName(Foundation, &BestPoiName);
			if (LocationTags.GameplayTags.Contains(BestPoiName))
			{
				FoundFoundation = Foundation;
				break;
			}
		}
	}
	printf("FoundFoundation:%s\n", FoundFoundation->GetName().c_str());

	if (!FoundFoundation)
		return Originals::PostInitializeComponents(__this);

	__this->BrushComponent = NewObject<UBrushComponent>(__this);
	__this->BrushComponent->K2_SetRelativeTransform(FoundFoundation->GetTransform(), false, nullptr, true);
	static void (*SetCollisonProfileName)(UBrushComponent*, __int64, __int64) = decltype(SetCollisonProfileName)(__this->BrushComponent->VTable[(0x620 / 8)]);
	RegisterComponent(__this->BrushComponent);
	SetCollisonProfileName(__this->BrushComponent, *reinterpret_cast<__int64*>(InSDKUtils::GetImageBase() + 0x8AD1210), 1);
	__this->BrushComponent->bGenerateOverlapEvents = true;
	SetGenerateOverlapEvents(__this->BrushComponent, true);

	UBodySetup* BodySetup = (UBodySetup*)UGameplayStatics::SpawnObject(UBodySetup::StaticClass(), __this->BrushComponent);
	BodySetup->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
	BodySetup->bGenerateMirroredCollision = false;
	BodySetup->bDoubleSidedGeometry = true;

	__this->BrushComponent->Brush = __this->Brush;
	__this->BrushComponent->BrushBodySetup = BodySetup;

	SetupAttachment(__this->PoiCollisionComp, __this->BrushComponent, false);

	FVector Origin{};
	FVector BoxExtent{};
	FoundFoundation->GetActorBounds(false, &Origin, &BoxExtent, true);
	auto& BoxBounds = *reinterpret_cast<FBoxSphereBounds*>(__int64(__this->BrushComponent) + 0x100);
	BoxBounds.BoxExtent = BoxExtent;
	BoxBounds.Origin = Origin;
	
	FKBoxElem Elem{};
	Elem.Center = FoundFoundation->GetCentroid();
	Elem.X = BoxBounds.BoxExtent.X;
	Elem.Y =BoxBounds.BoxExtent.Y;
	Elem.Z = BoxBounds.BoxExtent.Z;
	__this->BrushComponent->BrushBodySetup->AggGeom.BoxElems.Add(Elem);
	
	return Originals::PostInitializeComponents(__this);
}

void FortPoiVolume::Setup()
{
	UFortHookingLibrary::Get()->Hook(0x1E4F5B0, PostInitializeComponents, Originals::PostInitializeComponents);
}