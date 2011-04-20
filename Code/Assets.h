#pragma once

#include "Asset.h"
#include <list>
#include <string>

namespace Monocle
{
	class TextureAsset;
    class FontAsset;
    class AudioAsset;
    class AudioDecoder;

	class Assets
	{
	public:
		Assets();
		void Init();
		//static Asset *RequestAsset(AssetType assetType, const char *filename);
		static TextureAsset *RequestTexture(const std::string &filename, FilterType filter = FILTER_LINEAR, bool repeatX = false, bool repeatY = false);
        static FontAsset *RequestFont(const std::string &filename, float size, int textureWidth=-1, int textureHeight=-1);
		static AudioAsset *RequestAudio(const std::string &filename, AudioDecoder &decoder);

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