// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SyloDID.generated.h"

USTRUCT()
struct FSyloDID
{
	GENERATED_BODY()
public:
	FSyloDID() {}
	FSyloDID(const FString& SyloDID)
	{
		ParseDID(SyloDID);
	}

	void ParseDID(const FString& SyloDID);

	bool bValidDID = false;
	
	FString DID;
	FString SyloDataMethod;
	FString DataOwner;
	FString DataId;
	
	TMap<FString, FString> Args;
};
