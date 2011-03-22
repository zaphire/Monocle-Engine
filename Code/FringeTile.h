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
		~FringeTileset();
		
		bool IsName(const std::string &name);

		const FringeTileData *GetFringeTileDataByID(int tileID);
		void SetFringeTileData(int tileID, FringeTileData *fringeTileData);
		int GetNextTileID(int tileID);
		int GetPrevTileID(int tileID);

	private:
		int maxTileID;
		std::string name;
		std::map<int, FringeTileData*> fringeTileData;
	};

	class FringeTile : public Sprite
	{
	public:
		FringeTile(FringeTileset *fringeTileset, int tileID);
		void SetTileID(int tileID);
		int GetTileID();
		void PrevTile();
		void NextTile();
		FringeTileset *GetFringeTileset();

	private:
		void RefreshTexture();
		int tileID;
		FringeTileset *fringeTileset;
	};
}