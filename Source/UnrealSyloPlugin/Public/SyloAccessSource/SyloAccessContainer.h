#pragma once

class ISyloAccessSource;

class FSyloAccessContainer
{
public:
	void SetSyloAccessSource(const FString& Sylo, const TSharedPtr<ISyloAccessSource>& AccessSource);

	TSharedPtr<ISyloAccessSource> GetAccessSource(const FString& Sylo) const;
private:
	TMap<FString, TSharedPtr<ISyloAccessSource>> SourceMap;
};
