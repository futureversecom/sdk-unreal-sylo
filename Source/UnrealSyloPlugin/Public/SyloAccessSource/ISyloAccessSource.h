#pragma once

/**
 * Interface for providing access tokens and refreshing expired access tokens to be used while querying Sylo DIDs
 */
class UNREALSYLOPLUGIN_API ISyloAccessSource
{
public:
	virtual ~ISyloAccessSource() = default;

	/* Access token to be used as bearer token */
	virtual FString GetAccessToken() = 0;
	/* Async callback to refresh access token in the event it is expired */
	virtual TFuture<bool> RefreshAccessToken() = 0;
};
