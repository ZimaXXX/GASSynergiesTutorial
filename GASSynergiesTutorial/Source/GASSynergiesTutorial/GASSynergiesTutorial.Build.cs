// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASSynergiesTutorial : ModuleRules
{
	public GASSynergiesTutorial(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTags", "GameplayAbilities", "GameplayTasks", "PhysicsCore" });
	}
}
