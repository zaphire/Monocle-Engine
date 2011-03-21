#include "Ogmosis.h"

namespace Monocle
{
	// T I L E S E T

	Tileset::Tileset(const std::string &name, const std::string &filename)
		: texture(NULL)
	{
	}

	// T I L E M A P

	Tilemap::Tilemap(const std::string &filename, int width, int height, int tileWidth, int tileHeight)
		: Graphic()
	{
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
		tiles.resize(width*height);
	}

	void Tilemap::Resize(int width, int height)
	{
		this->width = width;
		this->height = height;

		/// TODO: resize array
		//std::vector<int> newTiles = std::vector<int>();
		//tiles.resize(width*height);
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
		for (int tx = 0; tx < width; tx ++)
		{
			for (int ty = 0; ty < height; ty++)
			{
				int tileID = tiles[ty*width + tx];
				
				// get x/y coords of tile into tilemap
				
				// convert to texture coordinates

				// render quad with texture coords set
			}
		}
	}

	// L E V E L

	Level::Level()
		: scene(NULL)
	{
	}

	void Level::SetScene(Scene *scene)
	{
		this->scene = scene;
	}

	void Level::Load(const std::string &filename)
	{
		// load from an xml file, into the scene

		if (scene)
		{

		}
	}

	void Level::Save()
	{
		// save our data out to xml file
	}

	void Level::SaveAs(const std::string &filename)
	{
		// save under a new filename
	}

	// O G M O S I S

	void Ogmosis::Begin()
	{
		Scene::Begin();

		level.SetScene(this);
		level.Load("test");
	}

	void Ogmosis::Update()
	{
		Scene::Update();
	}

	void Ogmosis::End()
	{
		Scene::End();
	}
}