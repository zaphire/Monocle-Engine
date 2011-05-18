#include "FringeTile.h"
#include "../Assets.h"
#include "../Level.h"

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

	FringeTile::FringeTile()
		: Entity(), tileID(0), sprite(NULL) //fringeTileset(NULL), tileID(0), sprite(NULL)
	{
		//AddTag("FringeTile");
		sprite = new Sprite();
		SetGraphic(sprite);

		RefreshTexture();
	}

	FringeTile::FringeTile(const FringeTile &fringeTile)
		: Entity(fringeTile), tileID(fringeTile.tileID), sprite(NULL)
	{
		//AddTag("FringeTile");
		sprite = new Sprite();
		sprite->blend = fringeTile.sprite->blend;
		SetGraphic(sprite);
		
		RefreshTexture();
	}

	Entity* FringeTile::Clone()
	{
		return new FringeTile(*this);
	}

	/*
	FringeTile::FringeTile(int tileID)
		: Entity(), tileID(tileID), sprite(NULL)
	{
		AddTag("FringeTile");

		sprite = new Sprite();
		SetGraphic(sprite);

		RefreshTexture();
	}
	*/
	
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
		tileID = Level::GetCurrentFringeTileset()->GetNextTileID(tileID);
		RefreshTexture();
	}

	void FringeTile::PrevTile()
	{
		tileID = Level::GetCurrentFringeTileset()->GetPrevTileID(tileID);
		RefreshTexture();
	}

	/*
	FringeTileset *FringeTile::GetFringeTileset()
	{
		return fringeTileset;
	}
	*/

	void FringeTile::NextBlend()
	{
		int spriteBlend = (int)sprite->blend;
		spriteBlend++;
		sprite->blend = (BlendType)spriteBlend;
	}

	void FringeTile::PrevBlend()
	{
		int spriteBlend = (int)sprite->blend;
		spriteBlend--;
		sprite->blend = (BlendType)spriteBlend;
	}

	void FringeTile::RefreshTexture()
	{
		printf("RefreshTexture to tileID: %d\n", tileID);

		// free old texture here somehow:
		if (sprite->texture)
		{
			sprite->texture->RemoveReference();
			sprite->texture = NULL;
		}

		const FringeTileData *fringeTileData = Level::GetCurrentFringeTileset()->GetFringeTileDataByID(tileID);
		if (fringeTileData)
		{
			sprite->texture = Assets::RequestTexture(fringeTileData->imageFilename, fringeTileData->filter, fringeTileData->repeatX, fringeTileData->repeatY);
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

	void FringeTile::Save(FileNode *fileNode)
	{
		Entity::Save(fileNode);
		fileNode->Write("tileID", tileID);
		if (sprite->blend != BLEND_ALPHA)
			fileNode->Write("blend", (int)sprite->blend);
	}

	void FringeTile::Load(FileNode *fileNode)
	{
		Entity::Load(fileNode);
		
		fileNode->Read("tileID", tileID);
		
		int spriteBlend = BLEND_ALPHA;
		fileNode->Read("blend", spriteBlend);
		sprite->blend = (BlendType)spriteBlend;

		RefreshTexture();
	}

}
