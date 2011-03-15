#pragma once

#include "Asset.h"
#include <list>
#include <string>

namespace Monocle
{
	class TextureAsset;

	class AssetDatabase
	{
	public:
		AssetDatabase();
		void Init();
		//static Asset *RequestAsset(AssetType assetType, const char *filename);
		static TextureAsset *RequestTexture(const std::string &filename);

		void StoreAsset(Asset *asset); // TODO: make private

	private:
		Asset* GetAssetByFilename(const std::string &filename);

		static AssetDatabase *instance;
		std::list<Asset*> assets;
		
	};
}