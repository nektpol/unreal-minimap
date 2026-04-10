// Developed by Neko Creative Technologies


#include "MM_POI_Widget.h"

#include "MM_Actor.h"
#include "PointOfInterestComponent.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Throbber.h"

void UMM_POI_Widget::SetPoiWidget(const bool& bIsStaticIn,const bool& bUseRotationIn, AActor* OwnerIn, AMM_Actor* MinimapActorIn)
{
	bStatic = bIsStaticIn;
	bUseRotation = bUseRotationIn;
	Owner = OwnerIn;
	MinimapActor = MinimapActorIn;
	
	UPointOfInterestComponent* PoiComp = Owner->FindComponentByClass<UPointOfInterestComponent>();
	if (PoiComp == nullptr) return;
	UTexture2D* IconTexture = PoiComp->GetIconTexture();
	if (IconTexture == nullptr)
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	IconImage->SetBrushFromTexture(PoiComp->GetIconTexture());
	Throbber->SetVisibility(ESlateVisibility::Collapsed);
}

void UMM_POI_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	// Outer widget must always remain Visible so Slate keeps ticking it.
	// Range-based hide/show is controlled on the inner Overlay instead.
	SetVisibility(ESlateVisibility::Visible);
}

void UMM_POI_Widget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Throbber == nullptr) return;

	Throbber->NumberOfPieces = 1;
	
	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor::Red);
	Throbber->SetImage(Brush);
}

void UMM_POI_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (MinimapActor == nullptr || Owner == nullptr)
	{
		SetRenderTranslation(FVector2D::ZeroVector);
		return;
	}

	const FVector OwnerLocation = Owner->GetActorLocation();
	const FVector MinimapLocation = MinimapActor->GetActorLocation();
	const FVector2D DeltaWorldXY(OwnerLocation.X - MinimapLocation.X, OwnerLocation.Y - MinimapLocation.Y);

	const float UnrealUnitsPerMapUnit = MinimapActor->GetUnrealUnitsPerMapUnit();
	if (UnrealUnitsPerMapUnit <= KINDA_SMALL_NUMBER)
	{
		SetRenderTranslation(FVector2D::ZeroVector);
		return;
	}

	const FVector2D DeltaMapUnits = DeltaWorldXY / UnrealUnitsPerMapUnit;
	SetRenderTranslation(FindCoord(DeltaMapUnits.Size(), FMath::RadiansToDegrees(FMath::Atan2(DeltaMapUnits.Y, DeltaMapUnits.X)), MinimapActor->GetMapSize()));
	
	if (!bStatic && Overlay != nullptr)
	{
		// Control the CONTENT visibility, never the outer widget.
		// Calling SetVisibility on 'this' stops Slate from ticking the widget,
		// so it would never recover when re-entering range.
		const bool bOutOfRange = DeltaWorldXY.SizeSquared() > FMath::Square(MinimapActor->GetOrthoWidth() * 0.5f);
		Overlay->SetVisibility(bOutOfRange ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
	
	if (bUseRotation)
	{
		IconImage->SetRenderTransformAngle(Owner->GetActorRotation().Yaw);
	}
}

FVector2D UMM_POI_Widget::FindCoord(const float& Radius, const float& Degrees, const float& MapSize)
{
	const float ClampedRadius = FMath::Clamp(Radius, 0.0f, MapSize * 0.5f);
	const float Radians = FMath::DegreesToRadians(Degrees);

	return FVector2D(FMath::Sin(Radians) * ClampedRadius, -FMath::Cos(Radians) * ClampedRadius);
}
