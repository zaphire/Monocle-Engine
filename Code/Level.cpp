#include "Level.h"
#include "Assets.h"
#include "XML/tinyxml.h"
#include <iostream>
#include <fstream>
#include "Entity.h"

/// TODO: move this somewhere common
#define min(a, b) (((a) < (b)) ? (a) : (b)) 
#define max(a, b) (((a) > (b)) ? (a) : (b)) 

namespace Monocle
{
	// T I L E S E T

	Tileset::Tileset(const std::string &name, const std::string &filename, float tileWidth, float tileHeight)
		: texture(NULL), tileWidth(tileWidth), tileHeight(tileHeight)
	{
		texture = Assets::RequestTexture(filename, FILTER_NONE);
	}

	// T I L E M A P

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
		int maxX = min(width, this->width);
		int maxY = min(height, this->height);
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
			Graphics::RenderQuad(400, 400);

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
					//Graphics::RenderQuad(tileWidth, tileHeight, Vector2(0,0), Vector2(0.125f,0.125f));
				}
			}
		}
	}

	// L E V E L

	Level *Level::instance = NULL;

	Level::Level()
		: scene(NULL)
	{
		instance = this;
		width = height = 0;
	}

	void Level::Init()
	{
	}

	void Level::SetScene(Scene *scene)
	{
		instance->scene = scene;
	}

	void Level::LoadProject(const std::string &filename)
	{
		// load the project data from an xml file
		instance->tilesets.clear();

		TiXmlDocument xml(Assets::GetContentPath() + filename);
		bool isLoaded = xml.LoadFile();
		if (isLoaded)
		{
			TiXmlElement* eProject = xml.FirstChildElement("Project");
			if (eProject)
			{
				TiXmlElement* eTilesets = eProject->FirstChildElement("Tilesets");
				if (eTilesets)
				{
					TiXmlElement* eTileset = eTilesets->FirstChildElement("Tileset");
					while (eTileset)
					{
						instance->tilesets.push_back(Tileset(XMLString(eTileset, "name"), XMLString(eTileset, "image"), XMLInt(eTileset, "tileWidth"), XMLInt(eTileset, "tileHeight")));

						eTileset = eTilesets->NextSiblingElement("Tileset");
					}
				}
			}
		}
	}

	void Level::Load(const std::string &filename)
	{
		// load from an xml file, into the scene

		if (instance->scene)
		{
			TiXmlDocument xml(Assets::GetContentPath() + filename);
			bool isLoaded = xml.LoadFile();

			if (isLoaded)
			{
				TiXmlElement* eLevel = xml.FirstChildElement("Level");
				if (eLevel)
				{
					instance->width = XMLInt(eLevel, "width");
					instance->height = XMLInt(eLevel, "height");

					TiXmlElement *eTilemap = eLevel->FirstChildElement("Tilemap");
					while (eTilemap)
					{
						Debug::Log("********************** adding tilemap entity");
						Entity *entity = new Entity();
						entity->SetGraphic(new Tilemap(instance->GetTileset(XMLString(eTilemap, "name")), instance->width, instance->height, 16, 16));
						instance->scene->Add(entity);

						TiXmlElement *eTile = eTilemap->FirstChildElement("Tile");
						while (eTile)
						{
							eTile = eTilemap->NextSiblingElement("Tile");
						}
						eTilemap = eLevel->NextSiblingElement("Tilemap");
					}
				}
			}
					
		}
	}

	Tileset *Level::GetTileset(const std::string &name)
	{
		for (std::list<Tileset>::iterator i = tilesets.begin(); i != tilesets.end(); ++i)
		{
			if ((*i).name == name)
				return &(*i);
		}
		Debug::Log("Error: Could not find tileset with name: " + name);
		return NULL;
	}

	void Level::Save()
	{
		// save our data out to xml file
	}

	void Level::SaveAs(const std::string &filename)
	{
		// save under a new filename
	}

	void Level::End()
	{
		// do any cleanup required

		instance->scene = NULL;
	}
}