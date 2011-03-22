#include "Asset.h"
#include "Debug.h"
#include "Assets.h"

namespace Monocle
{
	Asset::Asset(AssetType type)
		: type(type)
	{
		isCached = false;
		referenceCount = 0;
		type = ASSET_UNDEFINED;
	}

	void Asset::AddReference()
	{
		referenceCount++;

		//Debug::Log("referenceCount increased for: " + filename);
		//Debug::Log(referenceCount);
	}

	void Asset::RemoveReference()
	{
		referenceCount--;

		Debug::Log("referenceCount decreased for: " + filename);
		Debug::Log(referenceCount);

		if (referenceCount == 0)
		{
			if (!isCached)
			{
				Unload();
				Assets::RemoveAsset(this);
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