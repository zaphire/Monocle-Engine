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

				TiXmlElement* eFringeTilesets = eProject->FirstChildElement("FringeTilesets");
				if (eFringeTilesets)
				{
					TiXmlElement* eFringeTileset = eFringeTilesets->FirstChildElement("FringeTileset");
					while (eFringeTileset)
					{
						FringeTileset fringeTileset = FringeTileset(XMLString(eFringeTileset, "name"));
						
						TiXmlElement* eFringeTile = eFringeTileset->FirstChildElement("FringeTile");
						while (eFringeTile)
						{
							if (eFringeTile->Attribute("tileID") && eFringeTile->Attribute("image"))
							{
								int tileID = XMLInt(eFringeTile, "tileID");
								std::string image = XMLString(eFringeTile, "image");
								int width = -1;
								int height = -1;
								if (eFringeTile->Attribute("width") && eFringeTile->Attribute("height"))
								{
									width = XMLInt(eFringeTile, "width");
									height = XMLInt(eFringeTile, "height");
								}
								if (image != "")
								{
									fringeTileset.SetFringeTileData(tileID, FringeTileData(image, width, height));
								}
							}
							eFringeTile = eFringeTile->NextSiblingElement("FringeTile");
						}
						
						instance->fringeTilesets.push_back(fringeTileset);

						eFringeTileset = eFringeTileset->NextSiblingElement("FringeTileset");
					}
				}
			}
		}
	}

	void Level::Load(const std::string &filename, Scene* scene)
	{
		// load from an xml file, into the scene

		if (scene != NULL)
		{
			instance->scene = scene;
		}

		if (instance->scene)
		{
			// unload tilemaps... (need to destroy them?)
			instance->tilemaps.clear();
			//clearfringeTileset

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
							tilemap->SetTile(XMLInt(eTile, "x"), XMLInt(eTile, "y"), XMLInt(eTile, "tileID"));
							eTile = eTile->NextSiblingElement("Tile");
						}
						eTilemap = eTilemap->NextSiblingElement("Tilemap");
					}

					TiXmlElement *eFringeTiles = eLevel->FirstChildElement("FringeTiles");
					while (eFringeTiles)
					{
						FringeTileset *fringeTileset = instance->GetFringeTileset(XMLString(eFringeTiles, "set"));

						/*
						Entity *entity = new Entity();
						Tilemap *tilemap = new Tilemap(instance->GetTileset(XMLString(eTilemap, "set")), instance->width, instance->height, XMLInt(eTilemap, "tileWidth"), XMLInt(eTilemap, "tileHeight"));
						instance->tilemaps.push_back(tilemap);
						entity->SetGraphic(tilemap);
						instance->scene->Add(entity);
						*/

						if (fringeTileset)
						{
							TiXmlElement *eFringeTile = eFringeTiles->FirstChildElement("FringeTile");
							while (eFringeTile)
							{
								int tileID = XMLInt(eFringeTile, "tileID");
								int layer = XMLInt(eFringeTile, "layer");
								Entity *entity = new Entity();
								entity->SetLayer(layer);
								entity->position = Vector2(XMLFloat(eFringeTile, "x"), XMLFloat(eFringeTile, "y"));
								entity->rotation = XMLFloat(eFringeTile, "rotation");
								if (eFringeTile->Attribute("scaleX") && eFringeTile->Attribute("scaleY"))
									entity->scale = Vector2(XMLFloat(eFringeTile, "scaleX"), XMLFloat(eFringeTile, "scaleY"));
								FringeTile *fringeTile = new FringeTile(fringeTileset, tileID);
								instance->fringeTiles.push_back(fringeTile);
								entity->SetGraphic(fringeTile);
								instance->scene->Add(entity);

								//Debug::Log("loading... set tile");
								//tilemap->SetTile(XMLInt(eFringeTile, "x"), XMLInt(eFringeTile, "y"), XMLInt(eFringeTile, "tileID"));

								eFringeTile = eFringeTile->NextSiblingElement("FringeTile");
							}
						}

						eFringeTiles = eFringeTiles->NextSiblingElement("FringeTiles");
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

	FringeTileset *Level::GetFringeTileset(const std::string &name)
	{
		for (std::list<FringeTileset>::iterator i = fringeTilesets.begin(); i != fringeTilesets.end(); ++i)
		{
			return &(*i);
		}
		return NULL;
	}
}