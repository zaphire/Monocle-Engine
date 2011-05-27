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

	std::string Asset::GetName()
	{
	    //Will this stop working correctly if the content path is changed?
		int size = Assets::GetContentPath().size();
		return filename.substr(size, filename.size() - size);
	}
    
    std::string Asset::GetExtension()
    {
        std::string fn = GetName();
        return fn.substr(fn.find_last_of(".") + 1);
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
