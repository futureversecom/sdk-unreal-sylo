// Copyright (c) 2025, Futureverse Corporation Limited. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SyloLoadResult.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SyloAccessSource/ISyloAccessSource.h"
#include "SyloAccessSource/SyloAccessContainer.h"

#include "SyloSubsystem.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSyloLoadComplete, const FSyloLoadResult&, Result);

/**
 * API for loading data from sylo DIDs
 *
 * Load data with LoadSyloDID() or LoadSyloDIDFuture()
 *
 * Use SetSyloAccessSource() to provide Bearer tokens per Sylo Resolver (ResolverID)
 */
UCLASS()
class UNREALSYLOPLUGIN_API USyloSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/**
	 * Blueprint callable function for loading SyloData
	 * @param SyloDID Slyo DID to load data from
	 * @param OnSyloLoadComplete Triggered when data load is complete, provides FSyloLoadResult
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSyloDID(const FString& SyloDID, const FOnSyloLoadComplete& OnSyloLoadComplete);

	/**
	 * Loads Sylo data using Futures
	 * @param SyloDID Slyo DID to load data from
	 */
	TFuture<FSyloLoadResult> LoadSyloDIDFuture(const FString& SyloDID);


	/**
	 * Sets which access source to use per target Sylo
	 * @param Sylo Target Sylo or 'Sylo Resolver' or 'ResolverID'
	 * @param AccessSource SyloAccessSource interface which provides methods to get a access_token and refresh the token if it is expired
	 */
	void SetSyloAccessSource(const FString& Sylo, const TSharedPtr<ISyloAccessSource>& AccessSource);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
private:
	TSharedPtr<TArray<TSharedPtr<class FLoadSyloDataAction>>> ActiveLoads;
	TSharedPtr<class FSyloCache> Cache;

	TSharedPtr<FSyloAccessContainer> SyloAccessContainer;
};
