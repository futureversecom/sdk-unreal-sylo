#pragma once
#include "SyloDID.h"

namespace SyloUtils
{
	UNREALSYLOPLUGIN_API inline bool IsValidDID(const FString& DID)
	{
		return FSyloDID(DID).bValidDID;
	}
	
}
