// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.


#include "SyloFunctionLibrary.h"

void USyloFunctionLibrary::GetDataFromLoadResult(const FSyloLoadResult& LoadResult, TArray<uint8>& OutData)
{
	OutData = LoadResult.bSuccess && LoadResult.Data.IsValid() ? MoveTemp(*LoadResult.Data.Get()) : TArray<uint8>();
}
