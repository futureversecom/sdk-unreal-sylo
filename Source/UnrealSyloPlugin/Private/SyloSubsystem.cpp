// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.


#include "SyloSubsystem.h"

#include "LoadSyloDataAction.h"
#include "SyloCache.h"

void USyloSubsystem::LoadSyloDID(const FString& SyloDID, const FOnSyloLoadComplete& OnSyloLoadComplete)
{
	LoadSyloDIDFuture(SyloDID).Next([OnSyloLoadComplete](const FSyloLoadResult& Result)
	{
		OnSyloLoadComplete.ExecuteIfBound(Result);
	});
}

TFuture<FSyloLoadResult> USyloSubsystem::LoadSyloDIDFuture(const FString& SyloDID)
{
	TSharedPtr<TPromise<FSyloLoadResult>> Promise = MakeShared<TPromise<FSyloLoadResult>>();

	TSharedPtr<FLoadSyloDataAction> Action = MakeShared<FLoadSyloDataAction>();

	ActiveLoads->Add(Action);

	TSharedPtr<TArray<TSharedPtr<FLoadSyloDataAction>>> ActiveLoadsCopy = ActiveLoads;

	Action->LoadSyloDID(SyloDID, Cache).Next([Action, Promise, ActiveLoadsCopy](bool bResult)
	{
		FSyloLoadResult Result;
		Result.bSuccess = bResult;
		Result.Data = Action->GetData();

		ActiveLoadsCopy->Remove(Action);

		Promise->SetValue(Result);
	});

	return Promise->GetFuture();
}

void USyloSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Cache = MakeShared<FSyloCache>();
	ActiveLoads = MakeShared<TArray<TSharedPtr<FLoadSyloDataAction>>>();
}
