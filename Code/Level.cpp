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
			instance->tilemaps.clear();

			TiXmlDocument xml(Assets::GetContentPath() + filename);
			instance->filename = filename;
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
						Tilemap *tilemap = new Tilemap(instance->GetTileset(XMLString(eTilemap, "set")), instance->width, instance->height, XMLInt(eTilemap, "tileWidth"), XMLInt(eTilemap, "tileHeight"));
						instance->tilemaps.push_back(tilemap);
						entity->SetGraphic(tilemap);
						instance->scene->Add(entity);

						TiXmlElement *eTile = eTilemap->FirstChildElement("Tile");
						while (eTile)
						{
							Debug::Log("loading... set tile");
							tilemap->SetTile(XMLInt(eTile, "x"), XMLInt(eTile, "y"), XMLInt(eTile, "tileID"));
							eTile = eTile->NextSiblingElement("Tile");
						}
						eTilemap = eTilemap->NextSiblingElement("Tilemap");
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
		if (instance->filename == "")
		{
			// open save as dialog or something
		}
		else
		{
			// save our data out to xml file
			if (instance->scene)
			{
				TiXmlDocument xml;

				TiXmlElement eLevel("Level");
				eLevel.SetAttribute("width", instance->width);
				eLevel.SetAttribute("height", instance->height);

				if (!instance->tilemaps.empty())
				{
					//TiXmlElement eTilemaps("Tilemaps");
					for (std::list<Tilemap*>::iterator i = instance->tilemaps.begin(); i != instance->tilemaps.end(); ++i)
					{
						TiXmlElement eTilemap("Tilemap");
						if ((*i)->tileset)
							eTilemap.SetAttribute("set", (*i)->tileset->name);
						eTilemap.SetAttribute("tileWidth", (*i)->tileWidth);
						eTilemap.SetAttribute("tileHeight", (*i)->tileHeight);

						for (int ty = 0; ty < (*i)->height / (*i)->tileHeight; ty++)
						{
							for (int tx = 0; tx < (*i)->width / (*i)->tileWidth; tx++)
							{
								TiXmlElement eTile("Tile");
								eTile.SetAttribute("x", tx);
								eTile.SetAttribute("y", ty);
								eTile.SetAttribute("tileID", (*i)->GetTile(tx, ty));
								eTilemap.InsertEndChild(eTile);
							}
						}

						eLevel.InsertEndChild(eTilemap);
					}
					//eLevel.LinkEndChild(&eTilemaps);
				}
				xml.InsertEndChild(eLevel);

				xml.SaveFile(Assets::GetContentPath() + instance->filename);  
			}
		}
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