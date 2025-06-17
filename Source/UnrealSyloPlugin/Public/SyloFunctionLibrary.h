// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SyloLoadResult.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SyloFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSYLOPLUGIN_API USyloFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/* Used so blueprints can access data from FSyloLoadResult, invalidates load result data */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void GetDataFromLoadResult(const FSyloLoadResult& LoadResult, TArray<uint8>& OutData);
};
