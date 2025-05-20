// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SyloLoadResult.generated.h"

USTRUCT(BlueprintType)
struct FSyloLoadResult
{
	GENERATED_BODY()
public:
	UPROPERTY()
	bool bResult = false;

	/* Keep data as sharedptr to prevent needless copying of large amounts of data */
	TSharedPtr<TArray<uint8>> Data;
};