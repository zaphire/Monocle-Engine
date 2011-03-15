#include "Asset.h"

namespace Monocle
{
	Asset::Asset()
	{
		name = "\0";
		isCached = false;
		referenceCount = 0;
		type = AssetType::Undefined;
	}

	void Asset::AddReference()
	{
		referenceCount++;
	}

	void Asset::RemoveReference()
	{
		referenceCount--;

		if (referenceCount == 0)
		{
			if (!isCached)
			{
				Unload();
			}
		}
	}

	void Asset::Load(const char *filename)
	{

	}

	void Asset::Unload()
	{

	}
}