// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.


#include "SyloDID.h"

#include "Log/LogSylo.h"

void FSyloDID::ParseDID(const FString& SyloDID)
{
	// Reset
	SyloDataMethod.Empty();
	DataOwner.Empty();
	DataId.Empty();
	Args.Empty();

	DID = SyloDID;

	FString DIDCopy = DID;

	// Strip query parameters
	FString DIDMain;
	FString QueryParams;
	if (DIDCopy.Split(TEXT("?"), &DIDMain, &QueryParams))
	{
		TArray<FString> QueryPairs;
		QueryParams.ParseIntoArray(QueryPairs, TEXT("&"), true);
		for (const FString& Pair : QueryPairs)
		{
			FString Key, Value;
			if (Pair.Split(TEXT("="), &Key, &Value))
			{
				Args.Add(Key.ToLower(), Value.ToLower());
			}
		}
	}
	else
	{
		DIDMain = DIDCopy;
	}

	// Expected format: did:sylo-data:<owner>/<data-id>
	TArray<FString> DIDParts;
	DIDMain.ParseIntoArray(DIDParts, TEXT(":"), true);

	if (DIDParts.Num() < 3 || DIDParts[0].ToLower() != TEXT("did"))
	{
		UE_LOG(LogSylo, Verbose, TEXT("Invalid Sylo DID format: '%s'"), *DID);
		return;
	}

	SyloDataMethod = DIDParts[1];

	FString OwnerAndId = DIDParts[2];
	FString Owner, Id;
	if (OwnerAndId.Split(TEXT("/"), &Owner, &Id))
	{
		DataOwner = Owner;
		DataId = Id;
	}
	else
	{
		UE_LOG(LogSylo, Verbose, TEXT("Invalid Sylo DID body: '%s'"), *OwnerAndId);
		return;
	}

	bValidDID = true;
}
