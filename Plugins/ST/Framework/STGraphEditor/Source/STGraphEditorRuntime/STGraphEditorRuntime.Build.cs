// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class STGraphEditorRuntime : ModuleRules
{
	public STGraphEditorRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Engine",
		});
	}
}
