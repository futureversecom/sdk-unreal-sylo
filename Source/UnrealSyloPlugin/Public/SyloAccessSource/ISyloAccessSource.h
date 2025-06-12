#pragma once

class UNREALSYLOPLUGIN_API ISyloAccessSource
{
public:
	virtual ~ISyloAccessSource() = default;
	
	virtual FString GetAccessToken() = 0;
	virtual TFuture<bool> RefreshAccessToken() = 0;
};
