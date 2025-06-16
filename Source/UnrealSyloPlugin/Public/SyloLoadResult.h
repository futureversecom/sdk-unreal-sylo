// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SyloLoadResult.generated.h"

/**
 * Container holding results of a Sylo DID load.
 */
USTRUCT(BlueprintType)
struct UNREALSYLOPLUGIN_API FSyloLoadResult
{
	GENERATED_BODY()
public:
	/* Flag showing if the load failed or not */
	UPROPERTY()
	bool bSuccess = false;

	/* Keep data as sharedptr to prevent needless copying of large amounts of data, can be null if load failed */
	TSharedPtr<TArray<uint8>> Data;
};