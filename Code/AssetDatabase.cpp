#include "AssetDatabase.h"
#include "TextureAsset.h"
#include <stdio.h> // for NULL

namespace Monocle
{
	AssetDatabase *AssetDatabase::instance = NULL;

	AssetDatabase::AssetDatabase()
	{
		instance = this;
	}

	void AssetDatabase::Init()
	{
	}

	Asset* AssetDatabase::RequestAsset(AssetType assetType, const char *filename)
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

	TextureAsset *AssetDatabase::RequestTexture(const char *filename)
	{
		TextureAsset *newAsset = NULL;

		newAsset = new TextureAsset();
		newAsset->Load(filename);
		instance->StoreAsset((Asset*)newAsset);

		return newAsset;
	}

	void AssetDatabase::StoreAsset(Asset *asset)
	{
		// put in some list
	}
}