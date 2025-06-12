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
 * 
 */
UCLASS()
class UNREALSYLOPLUGIN_API USyloSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void LoadSyloDID(const FString& SyloDID, const FOnSyloLoadComplete& OnSyloLoadComplete);

	TFuture<FSyloLoadResult> LoadSyloDIDFuture(const FString& SyloDID);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void SetSyloAccessSource(const FString& Sylo, const TSharedPtr<ISyloAccessSource>& AccessSource);
private:
	TSharedPtr<TArray<TSharedPtr<class FLoadSyloDataAction>>> ActiveLoads;
	TSharedPtr<class FSyloCache> Cache;

	TSharedPtr<FSyloAccessContainer> SyloAccessContainer;
};
