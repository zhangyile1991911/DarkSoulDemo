// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DarkSoulDemo : ModuleRules
{
	public DarkSoulDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"GameplayTags", 
			"ModelViewViewModel",
			"UMG",
			"AIModule"
		});
	}
}
