#pragma once

#include "../Graphics/Sprite.h"
#include "../Entity.h"
#include "../Asset.h"
#include <string>
#include <map>

namespace Monocle
{
	class FringeTileData
	{
	public:
		FringeTileData()
			: width(-1), height(-1), repeatX(false), repeatY(false), autoTile(false)
		{}

		FringeTileData(const std::string &imageFilename, int width=-1, int height=-1, FilterType filter=FILTER_LINEAR, bool repeatX=false, bool repeatY=false, int atlasX=0, int atlasY=0, int atlasW=0, int atlasH=0, bool autoTile = false)
			: imageFilename(imageFilename), repeatX(repeatX), repeatY(repeatY), filter(filter), width(width), height(height), atlasX(atlasX), atlasY(atlasY), atlasW(atlasW), atlasH(atlasH), autoTile(autoTile)
		{}

		std::string imageFilename;
		int width, height;
		FilterType filter;
		bool repeatX, repeatY;
		int atlasX, atlasY, atlasW, atlasH;
		bool autoTile;
		// bool useFiltering;
	};

	class FringeTileset
	{
	public:
		FringeTileset(const std::string &name);
		~FringeTileset();
		
		bool IsName(const std::string &name);
		const std::string &GetName();

		const FringeTileData *GetFringeTileDataByID(int tileID);
		void SetFringeTileData(int tileID, FringeTileData *fringeTileData);
		int GetNextTileID(int tileID);
		int GetPrevTileID(int tileID);


	private:
		int maxTileID;
		std::string name;
		std::map<int, FringeTileData*> fringeTileData;
	};

	class FringeTile : public Entity
	{
	public:
		FringeTile();
		FringeTile(const FringeTile &fringeTile);
		Entity *Clone();
		//FringeTile(FringeTileset *fringeTileset, int tileID);
		void SetTileID(int tileID);
		int GetTileID();
		void PrevTile();
		void NextTile();
		void PrevBlend();
		void NextBlend();
		void Update();
		//FringeTileset *GetFringeTileset();
		Sprite *sprite;

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

	private:
		void RefreshTexture();
		void RefreshScale();
		int tileID;
		//FringeTileset *fringeTileset;
	};
}