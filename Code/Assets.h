#pragma once

#include <list>
#include <string>

#include "Asset.h"
#include "FontAsset.h"

namespace Monocle
{
	class TextureAsset;
    

	class Assets
	{
	public:
		Assets();
		void Init();
		//static Asset *RequestAsset(AssetType assetType, const char *filename);
		static TextureAsset *RequestTexture(const std::string &filename, FilterType filter = FILTER_LINEAR, bool repeatX = false, bool repeatY = false);
        static FontAsset *RequestFont(const std::string &filename, float size, int textureWidth=-1, int textureHeight=-1);
        static FontAsset *RequestFont(const std::string &filename, float size, int textureWidth, int textureHeight, const FontAsset::GlyphRanges& glyphRanges);
		//static AudioAsset *RequestAudio(const std::string &filename);

		static void SetContentPath(const std::string &contentPath);

		static const std::string &GetContentPath();

	protected:
		friend class Asset;
		static void RemoveAsset(Asset *asset);

	private:
		void StoreAsset(Asset *asset);

		Asset* GetAssetByFilename(const std::string &filename);


		static Assets *instance;
		std::list<Asset*> assets;

		std::string contentPath;
	};
}