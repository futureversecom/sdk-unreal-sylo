// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealSyloPlugin.h"

#define LOCTEXT_NAMESPACE "FUnrealSyloPluginModule"

void FUnrealSyloPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FUnrealSyloPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealSyloPluginModule, UnrealSyloPlugin)