// Developed by Neko Creative Technologies

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointOfInterestComponent.generated.h"


class AMM_Actor;
class UMM_Widget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIMAP_API UPointOfInterestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPointOfInterestComponent();
	virtual void BeginPlay() override;
		
	UFUNCTION(BlueprintCallable, Category = "Minimap|POI")
	void SetStatic(bool isStatic) { bStatic = isStatic; }
	UFUNCTION(BlueprintCallable, Category = "Minimap|POI")
	bool IsStatic() const { return bStatic; }
	UFUNCTION(BlueprintCallable, Category = "Minimap|POI")
	void SetUseRotation(bool bUseRotationIn) { bUseRotation = bUseRotationIn; }
	UFUNCTION(BlueprintCallable, Category = "Minimap|POI")
	bool IsUsingRotation() const { return bUseRotation; }
	
	UFUNCTION(BlueprintCallable, Category = "Minimap|POI")
	UTexture2D* GetIconTexture();
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Minimap|POI", meta = (AllowPrivateAccess = true))
	bool bStatic = false;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Minimap|POI", meta = (AllowPrivateAccess = true))
	bool bUseRotation = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Minimap|POI", meta = (AllowPrivateAccess = true))
	FDataTableRowHandle PointOfInterestDataRow;
	
	UPROPERTY()
	AMM_Actor* MinimapActor;
	
	
};
