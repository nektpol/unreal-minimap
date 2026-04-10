// Developed by Neko Creative Technologies


#include "MM_Widget.h"
#include "MM_POI_Widget.h"
#include "PointOfInterestComponent.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"

void UMM_Widget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (MinimapMaterial != nullptr)
	{
		MinimapImage->SetBrushFromMaterial(MinimapMaterial);
	}
}

void UMM_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}



void UMM_Widget::InitializeMinimapWidget(AMM_Actor* MinimapActorIn)
{
	MinimapActor = MinimapActorIn;
}

UMM_POI_Widget* UMM_Widget::AddPoi(AActor* Actor)
{
	UMM_POI_Widget* PoiWidget = CreateWidget<UMM_POI_Widget>(GetWorld(), PoiWidgetClass);
	bool bIsStatic = Actor->FindComponentByClass<UPointOfInterestComponent>()->IsStatic();
	bool bUseRotation = Actor->FindComponentByClass<UPointOfInterestComponent>()->IsUsingRotation();
	PoiWidget->SetPoiWidget(bIsStatic, bUseRotation, Actor, MinimapActor);
	UOverlaySlot* OverlaySlot = Overlay->AddChildToOverlay(PoiWidget);
	OverlaySlot->SetHorizontalAlignment(HAlign_Center);
	OverlaySlot->SetVerticalAlignment(VAlign_Center);
	return PoiWidget;
}

float UMM_Widget::GetMapSize()
{
	if (RootSizeBox == nullptr)
	{
		return 0.0f;
	}

	const float WidthOverride = RootSizeBox->GetWidthOverride();
	const float HeightOverride = RootSizeBox->GetHeightOverride();
	if (WidthOverride > 0.0f && HeightOverride > 0.0f)
	{
		return FMath::Min(WidthOverride, HeightOverride);
	}

	const FVector2D RootSize = RootSizeBox->GetCachedGeometry().GetLocalSize();
	if (RootSize.X > 0.0f && RootSize.Y > 0.0f)
	{
		return FMath::Min(RootSize.X, RootSize.Y);
	}

	if (MinimapImage != nullptr)
	{
		const FVector2D ImageSize = MinimapImage->GetCachedGeometry().GetLocalSize();
		return FMath::Min(ImageSize.X, ImageSize.Y);
	}

	return 0.0f;
}
