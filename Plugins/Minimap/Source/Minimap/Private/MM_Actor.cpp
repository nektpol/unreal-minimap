// Developed by Neko Creative Technologies


#include "MM_Actor.h"

#include "MM_Widget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"


// Sets default values
AMM_Actor::AMM_Actor()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	
	SpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCaptureComponent2D");
	SceneCaptureComponent->SetupAttachment(SpringArm);
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;
	SceneCaptureComponent->LODDistanceFactor = 2.0f;
	SceneCaptureComponent->MaxViewDistanceOverride = 10000.f;
	
	FEngineShowFlags& Flags = SceneCaptureComponent->ShowFlags;
	Flags.SetSkeletalMeshes(false);
	Flags.SetPostProcessing(false);
	Flags.SetMotionBlur(false);
	Flags.SetBloom(false);
	Flags.SetAmbientOcclusion(false);
	Flags.SetAntiAliasing(false);
	Flags.SetDynamicShadows(false);
	Flags.SetFog(false);
	Flags.SetVolumetricFog(false);
	Flags.SetLensFlares(false);
}

void AMM_Actor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	SceneCaptureComponent->OrthoWidth = OrthoWidth;
	if (RenderTarget == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AMM_Actor::BeginPlay() - RenderTarget is null"));
		return;
	}
	
	SceneCaptureComponent->TextureTarget = RenderTarget;
}

void AMM_Actor::BeginPlay()
{
	Super::BeginPlay();
	
	// Set refresh timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AMM_Actor::UpdateMinimap,RefreshRate,true);
	
	SetMinimapInputEnabled(bUseInput);
	
	MinimapWidget = CreateWidget<UMM_Widget>(GetWorld(), WidgetClass);
	MinimapWidget->InitializeMinimapWidget(this);
	CheckForUninitializedPois();

}

void AMM_Actor::SetOrthoWidth(float NewOrthoWidth)
{
	OrthoWidth = FMath::Max(NewOrthoWidth, 1.0f);
	if (SceneCaptureComponent != nullptr)
	{
		SceneCaptureComponent->OrthoWidth = OrthoWidth;
	}
}

float AMM_Actor::GetUnrealUnitsPerMapUnit() const
{
	const float MapSize = MinimapWidget != nullptr ? MinimapWidget->GetMapSize() : 0.0f;
	if (MapSize <= KINDA_SMALL_NUMBER)
	{
		return FMath::Max(OrthoWidth, KINDA_SMALL_NUMBER);
	}
	return FMath::Max(OrthoWidth / MapSize, KINDA_SMALL_NUMBER);
}

void AMM_Actor::AddPoiOnMinimap(AActor* Actor)
{
	PoisActors.Add(Actor,nullptr);
	if(MinimapWidget != nullptr)
	{
		PoisActors[Actor] = MinimapWidget->AddPoi(Actor);
	}
}

float AMM_Actor::GetMapSize()
{
	return MinimapWidget->GetMapSize();
}

void AMM_Actor::SetMinimapInputEnabled(bool bEnable)
{
	if (bMinimapInputEnabled == bEnable)
	{
		return;
	}

	APlayerController* PlayerController = GetWorld() != nullptr ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (PlayerController == nullptr)
	{
		return;
	}

	if (bEnable)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (MinimapInputContext != nullptr)
				{
					Subsystem->AddMappingContext(MinimapInputContext, 0);
				}
			}
		}
		if (!bInputsAreBinded)
		{
			BindMinimapInput();
		}
	}
	else
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (MinimapInputContext != nullptr)
				{
					Subsystem->RemoveMappingContext(MinimapInputContext);
				}
			}
		}
	}

	bMinimapInputEnabled = bEnable;
}

void AMM_Actor::BindMinimapInput()
{
	APlayerController* PlayerController = GetWorld() != nullptr ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (PlayerController == nullptr)
	{
		return;
	}

	if (PlayerController->InputComponent == nullptr)
	{
		EnableInput(PlayerController);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	if (EnhancedInputComponent != nullptr)
	{
		ZoomBindingHandle = EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AMM_Actor::HandleMinimapZoom).GetHandle();
		OpenMapBindingHandle = EnhancedInputComponent->BindAction(ToggleMapAction, ETriggerEvent::Started, this, &AMM_Actor::HandleToggleMap).GetHandle();

		bInputsAreBinded = true;
	}
	
}

void AMM_Actor::HandleMinimapZoom(const FInputActionValue& InputValue)
{
	const float ZoomAxisValue = InputValue.Get<float>();
	if (FMath::IsNearlyZero(ZoomAxisValue))
	{
		return;
	}

	constexpr float ZoomStep = 150.0f;
	SetOrthoWidth(FMath::Max(OrthoWidth - (ZoomAxisValue * ZoomStep), 1.0f));
}

void AMM_Actor::HandleToggleMap()
{
	OnToggleMap.Broadcast();
}



void AMM_Actor::UpdateMinimap()
{
	SceneCaptureComponent->CaptureScene();
}

void AMM_Actor::CheckForUninitializedPois()
{
	for (auto Poi : PoisActors)
	{
		if (Poi.Value == nullptr)
		{
			PoisActors[Poi.Key] = MinimapWidget->AddPoi(Poi.Key);
		}
	}
}
