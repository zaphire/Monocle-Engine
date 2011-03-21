#include "Tilemap.h"
#include "../Macros.h"
#include "../Debug.h"

namespace Monocle
{
	Tilemap::Tilemap(Tileset *tileset, int width, int height, int tileWidth, int tileHeight)
		: Graphic()
	{
		this->tileset = tileset;
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
		tiles.resize(width*height);
	}

	void Tilemap::Resize(int width, int height)
	{
		this->width = width;
		this->height = height;

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
		tiles = newTiles;
	}

	void Tilemap::GetWidthHeight(int *width, int *height)
	{
		*width = this->width;
		*height = this->height;
	}

	bool Tilemap::IsTile(int tx, int ty, int tileID)
	{
		return tiles[ty*width + tx] == tileID;
	}

	int Tilemap::GetTile(int tx, int ty)
	{
		return tiles[ty*width + tx];
	}

	void Tilemap::SetTile(int tx, int ty, int tileID)
	{
		tiles[ty*width + tx] = tileID;
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

	void Tilemap::Update()
	{
	}

	void Tilemap::Render()
	{
		if (this->tileset)
		{
			//hack: make graphics::center?
			Graphics::Translate(Vector2(400, 300));

			Graphics::BindTexture(tileset->texture);
			//Graphics::RenderQuad(400, 400);

			// TODO: divide width by tilew/h
			for (int tx = 0; tx < width; tx ++)
			{
				for (int ty = 0; ty < height; ty++)
				{
					int tileID = tiles[ty*width + tx];


				
					// get x/y coords of tile in tileset
				
					// convert to texture coordinates
					//float tex_x, tex_y;
					//tex_x = tileID/
				
					// render quad with texture coords set
					Debug::Log(tileset->tileWidth);

					Graphics::RenderQuad(tileset->tileWidth, tileset->tileHeight, Vector2(0,0), Vector2(1,1), Vector2(tx * tileset->tileWidth, ty * tileset->tileHeight));
				}
			}
		}
	}
}