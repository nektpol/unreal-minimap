// Developed by Neko Creative Technologies

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Actor.h"
#include "MM_Actor.generated.h"

class UMM_POI_Widget;
class UMM_Widget;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnToggleMap);

UCLASS()
class MINIMAP_API AMM_Actor : public AActor
{
	GENERATED_BODY()

public:
	AMM_Actor();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Minimap|Settings")
	void SetOrthoWidth(float NewOrthoWidth);

	UFUNCTION(BlueprintPure, Category = "Minimap|Settings")
	float GetOrthoWidth() const { return OrthoWidth; }

	// 1 map unit equals half of the current orthographic capture width.
	UFUNCTION(BlueprintPure, Category = "Minimap|Settings")
	float GetUnrealUnitsPerMapUnit() const;

	UFUNCTION(BlueprintCallable, Category = "Minimap|Input")
	void SetMinimapInputEnabled(bool bEnable);
	
	void AddPoiOnMinimap(AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	UMM_Widget* GetMinimapWidget();
	UFUNCTION(BlueprintCallable)
	UMM_Widget* GetMapWidget();
	
	float GetMapSize();
	
	UPROPERTY(BlueprintAssignable)
	FOnToggleMap OnToggleMap;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USceneCaptureComponent2D> SceneCaptureComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Minimap|References", meta = (AllowPrivateAccess = true))
	UTextureRenderTarget2D* RenderTarget = nullptr;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Minimap|References", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMM_Widget> WidgetClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Minimap|References", meta = (AllowPrivateAccess = true))
	UMaterialInterface* MinimapMaterial;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Minimap|References", meta = (AllowPrivateAccess = true))
	UMaterialInterface* MapMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|Settings", meta = (AllowPrivateAccess = true))
	float OrthoWidth = 1536.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|Settings", meta = (AllowPrivateAccess = true))
	float RefreshRate = 0.08f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|Settings", meta = (AllowPrivateAccess = true))
	bool bFollowPawn = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|Settings", meta = (AllowPrivateAccess = true))
	bool bUseInput = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|Settings|Input", meta = (AllowPrivateAccess = true))
	UInputMappingContext* MinimapInputContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|Settings|Input", meta = (AllowPrivateAccess = true))
	UInputAction* ZoomAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Minimap|Settings|Input", meta = (AllowPrivateAccess = true))
	UInputAction* ToggleMapAction;;
	
	UPROPERTY(BlueprintReadOnly, Category="Minimap|State", meta = (AllowPrivateAccess = true))
	bool bFollowActor = false;
	
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	void UpdateMinimap();

	void BindMinimapInput();
	void HandleMinimapZoom(const FInputActionValue& InputValue);
	void HandleToggleMap();
	
	UPROPERTY()
	UMM_Widget* MinimapWidget;

	UPROPERTY()
	TMap<AActor*, UMM_POI_Widget*> PoisActors;

	bool bMinimapInputEnabled = false;
	uint32 ZoomBindingHandle = 0;
	uint32 OpenMapBindingHandle = 0;
	
	void CheckForUninitializedPois();
	
	UPROPERTY()
	bool bInputsAreBinded = false;
	
};
