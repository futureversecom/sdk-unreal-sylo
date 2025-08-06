#include "LoadSyloDataAction.h"

#include "HttpModule.h"
#include "SyloUtils.h"
#include "Interfaces/IHttpResponse.h"
#include "Log/LogSylo.h"
#include "SyloAccessSource/ISyloAccessSource.h"

TFuture<bool> FLoadSyloDataAction::LoadSyloDID(const FString& InDID, const TSharedPtr<FSyloAccessContainer>& InAccessContainer, const TSharedPtr<FSyloCache>& SyloCache)
{
	LoadPromise = MakeShared<TPromise<bool>>();
	AccessContainer = InAccessContainer;
	check(AccessContainer.IsValid());

	if (!SyloUtils::IsValidDID(InDID))
	{
		LoadPromise->SetValue(false);
		return LoadPromise->GetFuture();
	}
	
	DID = InDID;

	TSharedPtr<FLoadSyloDataAction> SharedThis = AsShared();

	GetResolverID().Next([SharedThis](bool bSuccess)
	{
		if (!bSuccess)
		{
			SharedThis->HandleLoadFailure();
			return;
		}

		SharedThis->GetResolverEndpoint().Next([SharedThis](bool bSuccess)
		{
			if (!bSuccess)
			{
				SharedThis->HandleLoadFailure();
				return;
			}

			SharedThis->GetDataFromEndpoint().Next([SharedThis](bool bSuccess)
			{
				if (!bSuccess)
				{
					SharedThis->HandleLoadFailure();
					return;
				}

				SharedThis->HandleLoadSuccess();
			});
		});
	});
	return LoadPromise->GetFuture();
}

TFuture<bool> FLoadSyloDataAction::GetResolverID()
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

	if (DID.Args.Contains(TEXT("resolver")))
	{
		ResolverID = DID.Args[TEXT("resolver")];
		Promise->SetValue(true);
		return Promise->GetFuture();
	}

	// TODO query chainstate for this
	ResolverID = TEXT("fv-sylo-resolver-staging");
	Promise->SetValue(true);
	
	return Promise->GetFuture();
}

TFuture<bool> FLoadSyloDataAction::GetResolverEndpoint()
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

	// TODO query chainstate for this
	if (ResolverID == FString(TEXT("fv-sylo-resolver-dev")))
	{
		ResolverEndpoint = TEXT("https://sylo-resolver.data.storage-sylo.futureverse.dev");
		Promise->SetValue(true);
		return Promise->GetFuture();
	}

	if (ResolverID == FString(TEXT("fv-sylo-resolver-staging")))
	{
		ResolverEndpoint = TEXT("https://sylo-resolver.data.storage-sylo.futureverse.cloud");
		Promise->SetValue(true);
		return Promise->GetFuture();
	}

	if (ResolverID == FString(TEXT("fv-altered-state-sylo-staging")))
	{
		ResolverEndpoint = TEXT("https://altered-state.data.storage-sylo.futureverse.cloud");
		Promise->SetValue(true);
		return Promise->GetFuture();
	}

	if (ResolverID == FString(TEXT("fv-altered-state-sylo")))
	{
		ResolverEndpoint = TEXT("https://altered-state.data.storage-sylo.futureverse.app");
		Promise->SetValue(true);
		return Promise->GetFuture();
	}

	Promise->SetValue(false);
	return Promise->GetFuture();
}

TFuture<bool> FLoadSyloDataAction::GetDataFromEndpoint()
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

	TSharedPtr<ISyloAccessSource> SyloAccessSource = AccessContainer->GetAccessSource(ResolverID);

	bool bHasAuth = SyloAccessSource.IsValid();
	
	FString RequestURI = MakeRequestURI();
   
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	if (bHasAuth)
	{
		FString AuthType = TEXT("access_token");
		RequestURI = FString::Printf(TEXT("%s?authType=%s"), *RequestURI, *AuthType);
		FString BearerToken = SyloAccessSource->GetAccessToken();
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *BearerToken));
	}
	
	HttpRequest->SetURL(RequestURI);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("accept"), TEXT("*/*"));

	LogHttpRequest(HttpRequest);

	TSharedPtr<FLoadSyloDataAction> SharedThis = AsShared();

	HttpRequest->OnProcessRequestComplete().BindLambda(
		[Promise, SharedThis](FHttpRequestPtr Request, const FHttpResponsePtr& Response, bool bWasSuccessful)
		{
			if (!Response.IsValid())
			{
				UE_LOG(LogSylo, Error, TEXT("FLoadSyloDataAction::GetDataFromEndpoint received invalid response"));
				Promise->SetValue(false);
				return;
			}
			
			if (bWasSuccessful && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				UE_LOG(LogSylo, Verbose, TEXT("FLoadSyloDataAction::GetDataFromEndpoint succesfully retrieved data: %llu bytes"), Response->GetContentLength());
				
				*SharedThis->Data = Response->GetContent();
				Promise->SetValue(true);
			}
			else if (Response->GetResponseCode() == EHttpResponseCodes::Denied)
			{
				UE_LOG(LogSylo, Log, TEXT("FLoadSyloDataAction::GetDataFromEndpoint access token expired, attempting refresh ..."));
				
				SharedThis->RefreshAccessToken().Next([Promise, SharedThis](bool bSuccess)
				{
					if (!bSuccess)
					{
						Promise->SetValue(false);
						return;
					}

					// TODO handle possible infinite loop
					SharedThis->GetDataFromEndpoint().Next([Promise](bool bSuccess)
					{
						Promise->SetValue(bSuccess);
					});
				});
			}
			else
			{
				UE_LOG(LogSylo, Error, TEXT("FLoadSyloDataAction::GetDataFromEndpoint Failed to fetch Sylo data: Code: %s Response: %s"), 
					*EHttpResponseCodes::GetDescription((EHttpResponseCodes::Type)Response->GetResponseCode()).ToString(),
					*Response->GetContentAsString());
				
				Promise->SetValue(false);
			}
		}
	);

	HttpRequest->ProcessRequest();

	return Promise->GetFuture();
}

TFuture<bool> FLoadSyloDataAction::RefreshAccessToken()
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

	// Prevent infinite loop
	if (bRefreshTokenRequested)
	{
		UE_LOG(LogSylo, Warning, TEXT("FLoadSyloDataAction::RefreshAccessToken Refreshing token request twice in the same load action. Aborting ..."))
		Promise->SetValue(false);
		return Promise->GetFuture();
	}
	
	AccessContainer->GetAccessSource(ResolverID)->RefreshAccessToken().Next([Promise](bool bSuccess)
	{
		Promise->SetValue(bSuccess);
	});

	bRefreshTokenRequested = true;

	return Promise->GetFuture();
}

void FLoadSyloDataAction::HandleLoadFailure()
{
	LoadPromise->SetValue(false);
}

void FLoadSyloDataAction::HandleLoadSuccess()
{
	LoadPromise->SetValue(true);
}

void FLoadSyloDataAction::LogHttpRequest(const TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& HttpRequest) const
{
	FString Headers;

	for (auto Header : HttpRequest->GetAllHeaders())
	{
		Headers += Header + TEXT("&");
	}

	UE_LOG(LogSylo, Verbose, TEXT("Sylo data request: Type: %s | Headers: %s | URL: %s"), *HttpRequest->GetVerb(), *Headers, *HttpRequest->GetURL());
}

FString FLoadSyloDataAction::MakeRequestURI() const
{
	return FString::Printf(TEXT("%s/api/v1/objects/get/%s/%s"), *ResolverEndpoint, *DID.DataOwner, *DID.DataId);
}
