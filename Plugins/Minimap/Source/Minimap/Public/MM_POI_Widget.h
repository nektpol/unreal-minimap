// Developed by Neko Creative Technologies

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MM_POI_Widget.generated.h"

class AMM_Actor;
class UMM_Widget;

UCLASS()
class MINIMAP_API UMM_POI_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetPoiWidget(const bool& bIsStaticIn,const bool& bUseRotationIn, AActor* OwnerIn, AMM_Actor* MinimapActorIn);

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IconImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Overlay;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UThrobber> Throbber;
		
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> SizeBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap|Data", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	bool bStatic = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap|Data", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	bool bUseRotation = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap|Data", meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	AActor* Owner = nullptr;
		
	UPROPERTY()
	AMM_Actor* MinimapActor = nullptr;
	

	FVector2D FindCoord(const float& Radius, const float& Degrees, const float& MapSize);
	
};
