#pragma once

#include "Graphics/Sprite.h"
#include <string>
#include <map>

namespace Monocle
{
	class FringeTileData
	{
	public:
		FringeTileData()
			: width(-1), height(-1)
		{}

		FringeTileData(const std::string &imageFilename, int width=-1, int height=-1)
			: imageFilename(imageFilename), width(width), height(height)
		{}

		std::string imageFilename;
		int width, height;
	};

	class FringeTileset
	{
	public:
		FringeTileset(const std::string &name);
		const FringeTileData *GetFringeTileDataByID(int tileID);
		void SetFringeTileData(int tileID, const FringeTileData &fringeTileData);

	private:
		std::string name;
		std::map<int, FringeTileData> textures;
	};

	class FringeTile : public Sprite
	{
	public:
		FringeTile(FringeTileset *fringeTileset, int tileID);
		void SetTileID(int tileID);

	private:
		void RefreshTexture();
		int tileID;
		FringeTileset *fringeTileset;
	};
}