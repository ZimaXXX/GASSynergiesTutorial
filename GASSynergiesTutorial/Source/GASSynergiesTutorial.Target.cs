// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASSynergiesTutorialTarget : TargetRules
{
	public GASSynergiesTutorialTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		CppStandard = CppStandardVersion.Cpp20;
		ExtraModuleNames.Add("GASSynergiesTutorial");
	}
}
