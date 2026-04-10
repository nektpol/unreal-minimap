// Developed by Neko Creative Technologies

#pragma once

#include "CoreMinimal.h"
#include "PointOfInterestData.generated.h"

USTRUCT(BlueprintType)
struct FPointOfInterestData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap|POI")
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap|POI")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap|POI")
	UTexture2D* Icon = nullptr;
};
