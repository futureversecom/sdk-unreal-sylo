// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SyloLoadResult.generated.h"

USTRUCT(BlueprintType)
struct UNREALSYLOPLUGIN_API FSyloLoadResult
{
	GENERATED_BODY()
public:
	UPROPERTY()
	bool bSuccess = false;

	/* Keep data as sharedptr to prevent needless copying of large amounts of data */
	TSharedPtr<TArray<uint8>> Data;
};