#include "Tilemap.h"
#include "../Macros.h"
#include "../Graphics.h"
#include "../Debug.h"
#include "../Input.h"

namespace Monocle
{
	int Tilemap::selectedTile = 0;

	Tilemap::Tilemap(Tileset *tileset, int width, int height, int tileWidth, int tileHeight)
		: Graphic(), width(0), height(0), tileWidth(tileWidth), tileHeight(tileHeight)
	{
		this->tileset = tileset;

		Resize(width, height);
		Clear();

		this->selectedTile = 0;

		/// TEST:
		for (int y = 0; y < height/tileHeight; y++)
		{
			for (int x = 0; x < width/tileWidth; x++)
			{
				SetTile(x, y, (y*16) + x);
			}
		}
	}

	void Tilemap::Resize(int width, int height)
	{
		std::vector<int> newTiles = std::vector<int>(width*height);

		int maxX = MIN(width, this->width);
		int maxY = MIN(height, this->height);
		for(int x=0;x<maxX;++x)
		{
			for(int y=0;y<maxY;++y)
			{
				newTiles[y*width+x] = tiles[y*this->width+x];
			}
		}

		this->width = width;
		this->height = height;

		tiles = newTiles;
	}

	// defaults to -1 (no tile)
	void Tilemap::Clear(int tileID)
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				tiles[y * width + x] = tileID;
			}
		}
	}

	void Tilemap::GetWidthHeight(float *width, float *height)
	{
		*width = (float)this->width;
		*height = (float)this->height;
	}

	bool Tilemap::IsTile(int tx, int ty, int tileID)
	{
		return tiles[ty*width + tx] == tileID;
	}

	int Tilemap::GetTile(int tx, int ty)
	{
		//printf("GetTile(%d, %d) = %d\n", tx, ty, tiles[ty*width + tx]);
		return tiles[ty*width + tx];
	}

	void Tilemap::SetTile(int tx, int ty, int tileID)
	{
		tiles[ty*width + tx] = tileID;
		//printf("SetTile(%d, %d, %d)\n", tx, ty, tileID);
	}

	int Tilemap::GetTileAtWorldPosition(const Vector2 &position)
	{
		int tx, ty;
		WorldToTile(position, &tx, &ty);
		return GetTile(tx, ty);
	}

	void Tilemap::SetTileAtWorldPosition(const Vector2 &position, int tileID)
	{
		int tx, ty;
		WorldToTile(position, &tx, &ty);
		return SetTile(tx, ty, tileID);
	}

	void Tilemap::WorldToTile(const Vector2 &position, int *tx, int *ty)
	{
		*tx = (int)(position.x / (float)tileWidth);
		*ty = (int)(position.y / (float)tileHeight);
	}

	void Tilemap::UpdateEditor()
	{
		// hack editor
		Vector2 mousePosition = Input::GetMousePosition();
		int tx = mousePosition.x / tileWidth;
		int ty = mousePosition.y / tileWidth;

		//printf("mp (%d, %d)\n", tx, ty);

		if (Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT))
		{
			SetTile(tx, ty, selectedTile);
		}
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectedTile = GetTile(tx, ty);
		}
	}

	void Tilemap::Update()
	{
	}

	void Tilemap::Render(Entity *entity)
	{
		///HACK:
		UpdateEditor();

		if (this->tileset)
		{
			//Debug::Log("rendering!");
			Graphics::BindTexture(tileset->texture);

			Vector2 texOffset;
			Vector2 texScale = Vector2(tileset->tileWidth / (float)tileset->texture->width, tileset->tileHeight / (float)tileset->texture->height);
			int tilesPerRow = tileset->texture->width / tileset->tileWidth;

			for (int tx = 0; tx < width/tileWidth; tx ++)
			{
				for (int ty = 0; ty < height/tileHeight; ty++)
				{
					int tileID = tiles[(ty*width) + tx];

					//printf("%d, %d = %d", tx, ty, tileID);
					if (tileID != -1)
					{
						//printf("%d, %d = %d", tx, ty, tileID);

						// get x/y coords of tile in tileset
						int tileX = (tileID % tilesPerRow) * tileset->tileWidth;
						int tileY = (tileID / tilesPerRow) * tileset->tileHeight;

						texOffset = Vector2(tileX/(float)tileset->texture->width, tileY/(float)tileset->texture->height);
				
						// render quad with texture coords set
						Graphics::RenderQuad(tileWidth, tileHeight, texOffset, texScale, Vector2(tx * tileWidth + tileWidth*0.5f, ty * tileHeight + tileHeight*0.5f));
					}
				}
			}
		}
	}
}
