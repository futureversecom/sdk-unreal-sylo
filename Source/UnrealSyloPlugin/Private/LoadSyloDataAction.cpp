#include "LoadSyloDataAction.h"

#include "HttpModule.h"
#include "SyloPluginSettings.h"
#include "SyloUtils.h"
#include "Interfaces/IHttpResponse.h"
#include "Log/LogSylo.h"

TFuture<bool> FLoadSyloDataAction::LoadSyloDID(const FString& InDID, const TSharedPtr<FSyloCache>& SyloCache)
{
	LoadPromise = MakeShared<TPromise<bool>>();

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
	ResolverID = TEXT("fv-sylo-resolver-dev");
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

	Promise->SetValue(false);
	return Promise->GetFuture();
}

TFuture<bool> FLoadSyloDataAction::GetDataFromEndpoint()
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

	FString RequestURI = MakeRequestURI();
	FString BearerToken = GetDefault<USyloPluginSettings>()->TempBearerToken;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(RequestURI);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetHeader(TEXT("accept"), TEXT("*/*"));
	HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *BearerToken));

	HttpRequest->OnProcessRequestComplete().BindLambda(
		[Promise](FHttpRequestPtr Request, const FHttpResponsePtr& Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				// You could do something with the data here: Response->GetContent()
				UE_LOG(LogSylo, Verbose, TEXT("Sylo data retrieved: %llu bytes"), Response->GetContentLength());
				Promise->SetValue(true);
			}
			else
			{
				UE_LOG(LogSylo, Error, TEXT("Failed to fetch Sylo data: %s"), 
					Response.IsValid() ? *Response->GetContentAsString() : TEXT("No response"));
				Promise->SetValue(false);
			}
		}
	);

	HttpRequest->ProcessRequest();

	return Promise->GetFuture();
}

void FLoadSyloDataAction::HandleLoadFailure()
{
	LoadPromise->SetValue(false);
	LoadPromise = nullptr;
}

void FLoadSyloDataAction::HandleLoadSuccess()
{
	LoadPromise->SetValue(true);
	LoadPromise = nullptr;
}

FString FLoadSyloDataAction::MakeRequestURI() const
{
	return FString::Printf(TEXT("%s/api/objects/get/%s"), *ResolverEndpoint, *DID.DataId);
}
