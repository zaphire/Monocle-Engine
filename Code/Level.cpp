#include "Level.h"
#include "Assets.h"
#include "XML/tinyxml.h"
#include <iostream>
#include <fstream>
#include "Entity.h"

namespace Monocle
{
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
						Entity *entity = new Entity();
						entity->SetGraphic(new Tilemap(instance->GetTileset(XMLString(eTilemap, "name")), instance->width, instance->height, XMLInt(eTilemap, "tileWidth"), XMLInt(eTilemap, "tileHeight")));
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