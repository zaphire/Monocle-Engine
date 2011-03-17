#pragma once

#include "Asset.h"
#include <list>
#include <string>

namespace Monocle
{
	class TextureAsset;

	class Assets
	{
	public:
		Assets();
		void Init();
		//static Asset *RequestAsset(AssetType assetType, const char *filename);
		static TextureAsset *RequestTexture(const std::string &filename);
		//static AudioAsset *RequestAudio(const std::string &filename);

		void StoreAsset(Asset *asset); // TODO: make private

		static void SetContentPath(const std::string &contentPath);

	private:
		Asset* GetAssetByFilename(const std::string &filename);

		static Assets *instance;
		std::list<Asset*> assets;

		std::string contentPath;
	};
}