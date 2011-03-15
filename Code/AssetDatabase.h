#pragma once

#include "Asset.h"

namespace Monocle
{
	class TextureAsset;

	class AssetDatabase
	{
	public:
		AssetDatabase();
		void Init();
		static Asset *RequestAsset(AssetType assetType, const char *filename);
		static TextureAsset *RequestTexture(const char *filename);

		void StoreAsset(Asset *asset); // TODO: make private

	private:
		static AssetDatabase *instance;

		
	};
}