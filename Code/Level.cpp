#include "Level.h"
#include "Assets.h"
#include "XML/tinyxml.h"
#include "XML/XMLFileNode.h"
#include <iostream>
#include <fstream>
#include "Entity.h"
#include "LevelEditor/Node.h"

namespace Monocle
{
	Level *Level::instance = NULL;

	Level::Level()
		: scene(NULL), fringeTileset(NULL)
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
		Debug::Log("Loading Level...");
		// load the project data from an xml file
		instance->tilesets.clear();

		TiXmlDocument xml(Assets::GetContentPath() + filename);
		bool isLoaded = xml.LoadFile();
		if (isLoaded)
		{
			TiXmlElement* eProject = xml.FirstChildElement("Project");
			if (eProject)
			{
				// Load Tileset data (for tiles on a grid)
				TiXmlElement* eTilesets = eProject->FirstChildElement("Tilesets");
				if (eTilesets)
				{
					TiXmlElement* eTileset = eTilesets->FirstChildElement("Tileset");
					while (eTileset)
					{
						instance->tilesets.push_back(Tileset(XMLReadString(eTileset, "name"), XMLReadString(eTileset, "image"), XMLReadInt(eTileset, "tileWidth"), XMLReadInt(eTileset, "tileHeight")));

						eTileset = eTilesets->NextSiblingElement("Tileset");
					}
				}

				// Load FringeTileset data (for abitrarily sized 'n placed tiles)
				TiXmlElement* eFringeTilesets = eProject->FirstChildElement("FringeTilesets");
				if (eFringeTilesets)
				{
					TiXmlElement* eFringeTileset = eFringeTilesets->FirstChildElement("FringeTileset");
					while (eFringeTileset)
					{
						FringeTileset fringeTileset = FringeTileset(XMLReadString(eFringeTileset, "name"));
						
						TiXmlElement* eFringeTile = eFringeTileset->FirstChildElement("FringeTile");
						while (eFringeTile)
						{
							if (eFringeTile->Attribute("id") && eFringeTile->Attribute("image"))
							{
								int tileID = XMLReadInt(eFringeTile, "id");
								std::string image = XMLReadString(eFringeTile, "image");
								int width = -1;
								int height = -1;
								if (eFringeTile->Attribute("width") && eFringeTile->Attribute("height"))
								{
									width = XMLReadInt(eFringeTile, "width");
									height = XMLReadInt(eFringeTile, "height");
								}

								FilterType filter = FILTER_LINEAR;

								bool repeatX = XMLReadBool(eFringeTile, "repeatX");
								bool repeatY = XMLReadBool(eFringeTile, "repeatY");

								if (image != "")
								{
									fringeTileset.SetFringeTileData(tileID, new FringeTileData(image, width, height, filter, repeatX, repeatY));
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
		Debug::Log("...done");
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
					instance->width = XMLReadInt(eLevel, "width");
					instance->height = XMLReadInt(eLevel, "height");
					std::string fringeTilesetName = XMLReadString(eLevel, "fringeTileset");

					if (fringeTilesetName != "")
					{
						instance->fringeTileset = instance->GetFringeTilesetByName(fringeTilesetName);
					}

					TiXmlElement *eTilemap = eLevel->FirstChildElement("Tilemap");
					while (eTilemap)
					{
						Entity *entity = new Entity();
						Tilemap *tilemap = new Tilemap(instance->GetTilesetByName(XMLReadString(eTilemap, "set")), instance->width, instance->height, XMLReadInt(eTilemap, "tileWidth"), XMLReadInt(eTilemap, "tileHeight"));
						instance->tilemaps.push_back(tilemap);
						entity->SetGraphic(tilemap);
						instance->scene->Add(entity);

						TiXmlElement *eTile = eTilemap->FirstChildElement("Tile");
						while (eTile)
						{
							tilemap->SetTile(XMLReadInt(eTile, "x"), XMLReadInt(eTile, "y"), XMLReadInt(eTile, "tileID"));
							eTile = eTile->NextSiblingElement("Tile");
						}
						eTilemap = eTilemap->NextSiblingElement("Tilemap");
					}

					TiXmlElement *eEntities = eLevel->FirstChildElement("Entities");
					if (eEntities)
					{
						instance->LoadEntities(eEntities);
					}
				}
			}
					
		}
	}


	template <class T>
	void Level::SaveEntitiesOfType(const std::string &name, TiXmlElement *element)
	{
		XMLFileNode xmlFileNode;

		Entity *entity = scene->GetFirstEntity();
		while (entity)
		{
			T *t = dynamic_cast<T*>(entity);
			if (t)
			{
				TiXmlElement saveElement(name);
				xmlFileNode.element = &saveElement;

				//Entity *entity = dynamic_cast<Entity*>(t);
				entity->Save(&xmlFileNode);

				element->InsertEndChild(saveElement);
			}
			entity = scene->GetNextEntity();
		}
	}

	template <class T>
	void Level::LoadEntitiesOfType(const std::string &name, TiXmlElement *element)
	{
		XMLFileNode xmlFileNode;

		TiXmlElement *eEntity = element->FirstChildElement(name);
		while (eEntity)
		{
			T *t = new T();
			Entity *entity = dynamic_cast<Entity*>(t);
			scene->Add(entity);
			xmlFileNode.element = eEntity;
			entity->Load(&xmlFileNode);

			eEntity = eEntity->NextSiblingElement(name);
		}
	}

	void Level::LoadEntities(TiXmlElement *element)
	{
		if (element)
		{
			LoadEntitiesOfType<FringeTile>("FringeTile", element);
			LoadEntitiesOfType<Node>("Node", element);
		}
	}

	void Level::SaveEntities(TiXmlElement *element)
	{
		SaveEntitiesOfType<FringeTile>("FringeTile", element);
		SaveEntitiesOfType<Node>("Node", element);
	}

	Tileset *Level::GetTilesetByName(const std::string &name)
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
			Debug::Log("Warning: Won't save level, no filename set");
		}
		else
		{
			// save our data out to xml file
			if (instance->scene)
			{
				Debug::Log("Saving scene...");

				TiXmlDocument xml;

				TiXmlElement eLevel("Level");
				eLevel.SetAttribute("width", instance->width);
				eLevel.SetAttribute("height", instance->height);
				if (instance->fringeTileset)
				{
					eLevel.SetAttribute("fringeTileset", instance->fringeTileset->GetName());
				}

				if (!instance->tilemaps.empty())
				{
					//TiXmlElement eTilemaps("Tilemaps");

					// save tilemaps
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
				}

				TiXmlElement eEntities("Entities");
				instance->SaveEntities(&eEntities);
				eLevel.InsertEndChild(eEntities);

				/*
				// save fringe tiles
				// go through the sets
				for (std::list<FringeTileset>::iterator i = instance->fringeTilesets.begin(); i != instance->fringeTilesets.end(); ++i)
				{
					bool savedAny = false;
					TiXmlElement eFringeTiles("FringeTiles");
					eFringeTiles.SetAttribute("set", (*i).GetName());

					// save fringeTiles that belong to the set
					for (std::list<FringeTile*>::iterator j = instance->fringeTiles.begin(); j != instance->fringeTiles.end(); ++j)
					{
						if ((*j)->GetFringeTileset() == &(*i))
						{
							savedAny = true;

							TiXmlElement eFringeTile("FringeTile");

							// TODO: use tags instead
							eFringeTile.SetAttribute("id", (*j)->GetTileID());
							eFringeTile.SetAttribute("layer", (*j)->GetLayer());
							eFringeTile.SetAttribute("x", (*j)->position.x);
							eFringeTile.SetAttribute("y", (*j)->position.y);
							eFringeTile.SetAttribute("rotation", (*j)->rotation);
							eFringeTile.SetDoubleAttribute("scaleX", (*j)->scale.x);
							eFringeTile.SetDoubleAttribute("scaleY", (*j)->scale.y);

							if ((*j)->color.a != 1.0f)
							{
								eFringeTile.SetDoubleAttribute("ca", (*j)->color.a);
							}

							eFringeTiles.InsertEndChild(eFringeTile);
						}
					}

					if (savedAny)
					{
						eLevel.InsertEndChild(eFringeTiles);
					}
				}
				*/

				xml.InsertEndChild(eLevel);

				xml.SaveFile(Assets::GetContentPath() + instance->filename);  

				Debug::Log("...done");
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

	FringeTileset *Level::GetFringeTilesetByName(const std::string &name)
	{
		for (std::list<FringeTileset>::iterator i = fringeTilesets.begin(); i != fringeTilesets.end(); ++i)
		{
			if ((*i).IsName(name))
			{
				return &(*i);

			}
		}
		return NULL;
	}

	FringeTileset* Level::GetCurrentFringeTileset()
	{
		return instance->fringeTileset;
	}
}