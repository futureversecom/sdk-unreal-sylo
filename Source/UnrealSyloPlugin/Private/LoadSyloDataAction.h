#pragma once
#include "SyloCache.h"
#include "SyloDID.h"
#include "SyloAccessSource/SyloAccessContainer.h"

class IHttpRequest;

class FLoadSyloDataAction : public TSharedFromThis<FLoadSyloDataAction>
{
public:
	TFuture<bool> LoadSyloDID(const FString& InDID, const TSharedPtr<FSyloAccessContainer>& AccessContainer, const TSharedPtr<FSyloCache>& SyloCache);

	const TSharedPtr<TArray<uint8>>& GetData() const {return Data;}
	bool HasSuccessfullyLoadedData() const {return bLoadSuccess;}
private:
	TFuture<bool> GetResolverID();
	TFuture<bool> GetResolverEndpoint();
	TFuture<bool> GetDataFromEndpoint();
	
	TFuture<bool> RefreshAccessToken();
	
	void HandleLoadFailure();
	void HandleLoadSuccess();

	void LogHttpRequest(const TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& HttpRequest) const;

	FString MakeRequestURI() const;

	TSharedPtr<TPromise<bool>> LoadPromise;
	
	FSyloDID DID;
	
	FString ResolverID;
	FString ResolverEndpoint;

	bool bLoadSuccess = false;
	bool bRefreshTokenRequested = false;

	TSharedPtr<TArray<uint8>> Data = MakeShared<TArray<uint8>>();
	TSharedPtr<FSyloAccessContainer> AccessContainer;
};
