// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SyloPluginSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, DisplayName=Sylo)
class UNREALSYLOPLUGIN_API USyloPluginSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	USyloPluginSettings();
};
