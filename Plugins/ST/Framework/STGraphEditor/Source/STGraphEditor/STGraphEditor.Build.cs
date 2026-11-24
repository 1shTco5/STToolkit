// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class STGraphEditor : ModuleRules
{
	public STGraphEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"STGraphEditorRuntime",
			"AssetDefinition",
			"Engine",
			"UnrealEd",
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"AssetTools",
			"AssetRegistry",
			"GraphEditor",
			"Projects",
			"PropertyEditor",
			"Slate",
			"SlateCore",
			"ToolMenus",
		});
	}
}
