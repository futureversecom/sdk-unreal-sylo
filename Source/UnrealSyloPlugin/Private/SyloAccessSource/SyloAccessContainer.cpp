#include "SyloAccessSource/SyloAccessContainer.h"

void FSyloAccessContainer::SetSyloAccessSource(const FString& Sylo, const TSharedPtr<ISyloAccessSource>& AccessSource)
{
	SourceMap.Add(Sylo, AccessSource);
}

TSharedPtr<ISyloAccessSource> FSyloAccessContainer::GetAccessSource(const FString& Sylo) const
{
	return SourceMap.FindRef(Sylo);
}
