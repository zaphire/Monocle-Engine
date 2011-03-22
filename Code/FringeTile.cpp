#include "FringeTile.h"
#include "Assets.h"

namespace Monocle
{
	FringeTileset::FringeTileset(const std::string &name)
		: name(name)
	{
	}

	const FringeTileData *FringeTileset::GetFringeTileDataByID(int tileID)
	{
		return &textures[tileID];
	}

	void FringeTileset::SetFringeTileData(int tileID, const FringeTileData &fringeTileData)
	{
		textures[tileID] = fringeTileData;
	}

	FringeTile::FringeTile(FringeTileset *fringeTileset, int tileID)
		: Sprite(), fringeTileset(fringeTileset), tileID(tileID)
	{
		RefreshTexture();
	}
	
	void FringeTile::SetTileID(int tileID)
	{
		this->tileID = tileID;
		RefreshTexture();
	}

	void FringeTile::RefreshTexture()
	{
		if (fringeTileset)
		{
			// free old texture here somehow:
			if (texture)
			{
				texture->RemoveReference();
				texture = NULL;
			}

			const FringeTileData *fringeTileData = fringeTileset->GetFringeTileDataByID(tileID);
			if (fringeTileData)
			{
				texture = Assets::RequestTexture(fringeTileData->imageFilename);
				if (fringeTileData->width == -1 && fringeTileData->height == -1 && texture)
				{
					width = texture->width;
					height = texture->height;
				}
				else
				{
					width = fringeTileData->width;
					height = fringeTileData->height;
				}
			}
		}
	}
}