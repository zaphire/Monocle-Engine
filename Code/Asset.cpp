#include "Asset.h"

namespace Monocle
{
	Asset::Asset(AssetType type)
		: type(type)
	{
		name = "\0";
		isCached = false;
		referenceCount = 0;
		type = AT_UNDEFINED;
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

	/*
	void Asset::Load(const char *filename)
	{

	}

	void Asset::Unload()
	{

	}
	*/
}