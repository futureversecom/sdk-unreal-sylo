#pragma once
#include "SyloDID.h"

namespace SyloUtils
{
	inline bool IsValidDID(const FString& DID)
	{
		return FSyloDID(DID).bValidDID;
	}
	
}
