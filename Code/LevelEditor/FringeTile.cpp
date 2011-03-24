#include "FringeTile.h"
#include "../Assets.h"

#include <cstdio>

namespace Monocle
{
	FringeTileset::FringeTileset(const std::string &name)
		: name(name), maxTileID(0)
	{
	}

	FringeTileset::~FringeTileset()
	{
		// delete
	}

	bool FringeTileset::IsName(const std::string &name)
	{
		return this->name == name;
	}

	const std::string &FringeTileset::GetName()
	{
		return this->name;
	}

	const FringeTileData *FringeTileset::GetFringeTileDataByID(int tileID)
	{
		return fringeTileData[tileID];
	}

	void FringeTileset::SetFringeTileData(int tileID, FringeTileData *fringeTileData)
	{
		this->fringeTileData[tileID] = fringeTileData;
		if ((tileID+1) > maxTileID) maxTileID = tileID+1;
	}
	
	int FringeTileset::GetNextTileID(int tileID)
	{
		if (maxTileID < 1) return -1;

		do
		{
			tileID++;
			if (tileID >= maxTileID)
				tileID = 0;
		}
		while (!fringeTileData[tileID]);
		return tileID;
	}

	int FringeTileset::GetPrevTileID(int tileID)
	{
		if (maxTileID < 1) return -1;

		do
		{
			tileID--;
			if (tileID < 0)
				tileID = maxTileID-1;
		}
		while (!fringeTileData[tileID]);

		return tileID;
	}

	FringeTile::FringeTile(FringeTileset *fringeTileset, int tileID)
		: Entity(), fringeTileset(fringeTileset), tileID(tileID), sprite(NULL)
	{
		AddTag("FringeTile");

		sprite = new Sprite();
		SetGraphic(sprite);
				
		RefreshTexture();
	}
	
	void FringeTile::SetTileID(int tileID)
	{
		this->tileID = tileID;
		RefreshTexture();
	}

	int FringeTile::GetTileID()
	{
		return tileID;
	}

	void FringeTile::NextTile()
	{
		if (fringeTileset)
		{
			tileID = fringeTileset->GetNextTileID(tileID);
			RefreshTexture();
		}
	}

	void FringeTile::PrevTile()
	{
		if (fringeTileset)
		{
			tileID = fringeTileset->GetPrevTileID(tileID);
			RefreshTexture();
		}
	}

	FringeTileset *FringeTile::GetFringeTileset()
	{
		return fringeTileset;
	}

	void FringeTile::RefreshTexture()
	{
		if (fringeTileset)
		{
			printf("RefreshTexture to tileID: %d\n", tileID);

			// free old texture here somehow:
			if (sprite->texture)
			{
				sprite->texture->RemoveReference();
				sprite->texture = NULL;
			}

			const FringeTileData *fringeTileData = fringeTileset->GetFringeTileDataByID(tileID);
			if (fringeTileData)
			{
				sprite->texture = Assets::RequestTexture(fringeTileData->imageFilename);
				if (fringeTileData->width == -1 && fringeTileData->height == -1 && sprite->texture)
				{
					sprite->width = sprite->texture->width;
					sprite->height = sprite->texture->height;
				}
				else
				{
					sprite->width = fringeTileData->width;
					sprite->height = fringeTileData->height;
				}
			}
		}
	}
}
