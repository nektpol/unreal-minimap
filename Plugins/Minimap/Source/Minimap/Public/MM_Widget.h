// Developed by Neko Creative Technologies

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MM_Widget.generated.h"

class AMM_Actor;
class UMM_POI_Widget;
/**
 * 
 */
UCLASS()
class MINIMAP_API UMM_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeMinimapWidget(AMM_Actor* MinimapActorIn);
	
	UMM_POI_Widget* AddPoi(AActor* Actor);
	float GetMapSize();
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MinimapImage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Overlay;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> RootSizeBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|References", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMM_POI_Widget> PoiWidgetClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|References", meta = (AllowPrivateAccess = true))
	UMaterialInterface* MinimapMaterial = nullptr;
	
	UPROPERTY()
	AMM_Actor* MinimapActor = nullptr;
	
	
};
