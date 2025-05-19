#include "LoadSyloDataAction.h"

#include "SyloUtils.h"

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

	return Promise->GetFuture();
}

TFuture<bool> FLoadSyloDataAction::GetResolverEndpoint()
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

	return Promise->GetFuture();
}

TFuture<bool> FLoadSyloDataAction::GetDataFromEndpoint()
{
	TSharedPtr<TPromise<bool>> Promise = MakeShared<TPromise<bool>>();

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
