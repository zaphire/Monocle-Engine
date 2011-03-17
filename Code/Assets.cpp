#include "Assets.h"
#include "TextureAsset.h"
#include <stdio.h> // for NULL
#include "Debug.h"

namespace Monocle
{
	Assets *Assets::instance = NULL;

	Assets::Assets()
	{
		instance = this;
	}

	void Assets::Init()
	{
	}

	TextureAsset *Assets::RequestTexture(const std::string &filename)
	{
		TextureAsset *asset = NULL;
		std::string fullFilename = instance->contentPath + filename;

		Debug::Log("instance->contentPath + filename: " + fullFilename);

		// check to see if we have one stored already
		asset = (TextureAsset*)instance->GetAssetByFilename(fullFilename);

		// if not, load it and store it
		if (!asset)
		{
			asset = new TextureAsset();
			asset->Load(fullFilename);
			instance->StoreAsset((Asset*)asset);
		}

		if (asset)
		{
			asset->AddReference();
		}

		// return whatever we found
		return asset;
	}

	void Assets::StoreAsset(Asset *asset)
	{
		// put in some list
		assets.push_back(asset);
	}

	Asset *Assets::GetAssetByFilename(const std::string &filename)
	{
		for(std::list<Asset*>::iterator i = assets.begin(); i != assets.end(); ++i)
		{
			if ((*i)->filename.compare(filename) == 0)
			{
				return (*i);
			}
		}
		return NULL;
	}

	void Assets::SetContentPath(const std::string &contentPath)
	{
		instance->contentPath = contentPath;

		/*
		if (instance->contentPath[instance->contentPath.length()-1] != '/' && instance->contentPath[instance->contentPath.length()-1] != '\\')
		{
			instance->contentPath += '/';
		}
		*/
	}


	//generic request asset, not sure if we need this yet
	/*
	Asset* Assets::RequestAsset(AssetType assetType, const char *filename)
	{
		Asset *newAsset = NULL;

		switch(assetType)
		{
		case AT_TEXTURE:
			newAsset = (Asset*)new TextureAsset();
			newAsset->Load(filename);
			instance->StoreAsset((Asset*)newAsset);
			break;
		}

		return newAsset;
	}
	*/

}