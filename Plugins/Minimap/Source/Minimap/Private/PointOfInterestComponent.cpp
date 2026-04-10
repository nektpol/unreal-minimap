// Developed by Neko Creative Technologies


#include "PointOfInterestComponent.h"

#include "MM_Actor.h"
#include "PointOfInterestData.h"
#include "Kismet/GameplayStatics.h"


UPointOfInterestComponent::UPointOfInterestComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}

void UPointOfInterestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMM_Actor::StaticClass(), FoundActors);

	MinimapActor = Cast<AMM_Actor>(FoundActors[0]);
	
	if (MinimapActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UPointOfInterestComponent::InitializePOIComponent() - MinimapActor is null"));
		return;
	}
	
	MinimapActor->AddPoiOnMinimap(GetOwner());
}

UTexture2D* UPointOfInterestComponent::GetIconTexture()
{
	if (PointOfInterestDataRow.DataTable == nullptr) return nullptr;
	
	static const FString ContextString(TEXT("PointOfInterest Lookup"));

	FPointOfInterestData* Row = PointOfInterestDataRow.GetRow<FPointOfInterestData>(ContextString);
	
	if (Row == nullptr) return nullptr;
	return Row->Icon;
}








