// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealMinimap : ModuleRules
{
	public UnrealMinimap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UnrealMinimap",
			"UnrealMinimap/Variant_Platforming",
			"UnrealMinimap/Variant_Platforming/Animation",
			"UnrealMinimap/Variant_Combat",
			"UnrealMinimap/Variant_Combat/AI",
			"UnrealMinimap/Variant_Combat/Animation",
			"UnrealMinimap/Variant_Combat/Gameplay",
			"UnrealMinimap/Variant_Combat/Interfaces",
			"UnrealMinimap/Variant_Combat/UI",
			"UnrealMinimap/Variant_SideScrolling",
			"UnrealMinimap/Variant_SideScrolling/AI",
			"UnrealMinimap/Variant_SideScrolling/Gameplay",
			"UnrealMinimap/Variant_SideScrolling/Interfaces",
			"UnrealMinimap/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
