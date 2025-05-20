#pragma once
#include "SyloCache.h"
#include "SyloDID.h"

class FLoadSyloDataAction : public TSharedFromThis<FLoadSyloDataAction>
{
public:
	TFuture<bool> LoadSyloDID(const FString& InDID, const TSharedPtr<FSyloCache>& SyloCache);

	const TSharedPtr<TArray<uint8>>& GetData() const {return Data;}
	bool HasSuccessfullyLoadedData() const {return bLoadSuccess;}
private:
	TFuture<bool> GetResolverID();
	TFuture<bool> GetResolverEndpoint();
	TFuture<bool> GetDataFromEndpoint();

	void HandleLoadFailure();
	void HandleLoadSuccess();

	FString MakeRequestURI() const;

	TSharedPtr<TPromise<bool>> LoadPromise;
	
	FSyloDID DID;
	
	FString ResolverID;
	FString ResolverEndpoint;

	bool bLoadSuccess = false;

	TSharedPtr<TArray<uint8>> Data = MakeShared<TArray<uint8>>();
};
