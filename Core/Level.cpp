#include "Level.h"
#include "Assets.h"
#include <TinyXML/tinyxml.h>
#include "XML/XMLFileNode.h"
#include <iostream>
#include <fstream>
#include "Entity.h"
#include "LevelEditor/Node.h"
#include "LevelEditor/PathMesh.h"

namespace Monocle
{

	EntityType::EntityType()
	{
	}

	void EntityType::Save(FileNode *fileNode)
	{
		fileNode->Write("name", name);
		fileNode->Write("image", image);
	}

	void EntityType::Load(FileNode *fileNode)
	{
		fileNode->Read("name", name);
		fileNode->Read("image", image);
	}

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
		///TODO: clear fringeTileset

		TiXmlDocument xml(Assets::GetContentPath() + filename);
		bool isLoaded = xml.LoadFile();
		if (isLoaded)
		{
			TiXmlElement* eProject = xml.FirstChildElement("Project");
			if (eProject)
			{
				TiXmlElement* eEntityTypes = eProject->FirstChildElement("EntityTypes");
				if (eEntityTypes)
				{
					TiXmlElement* eEntityType = eEntityTypes->FirstChildElement("EntityType");
					while (eEntityType)
					{
						XMLFileNode xmlFileNode(eEntityType);

						EntityType entityType;
						entityType.Load(&xmlFileNode);

						instance->entityTypes.push_back(entityType);

						eEntityType = eEntityType->NextSiblingElement("EntityType");
					}
				}

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

				// Load FringeTileset data (for arbitrarily sized 'n placed tiles)
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

								int atlasX=0, atlasY=0, atlasW=0, atlasH=0;
								std::string atlas = XMLReadString(eFringeTile, "atlas");
								if (atlas != "")
								{
									std::istringstream is(atlas);
									is >> atlasX >> atlasY >> atlasW >> atlasH;
								}

								if (image != "")
								{
									fringeTileset.SetFringeTileData(tileID, new FringeTileData(image, width, height, filter, repeatX, repeatY, atlasX, atlasY, atlasW, atlasH));
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

					Color backgroundColor = Color::black;
					XMLReadColor(eLevel, "backgroundColor", &backgroundColor);
					Graphics::SetBackgroundColor(backgroundColor);

					XMLFileNode xmlFileNode(eLevel);
					instance->scene->LoadLevel(&xmlFileNode);

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
						instance->scene->LoadEntities(eEntities);
					}


					instance->scene->ResolveEntityChanges();
				}
			}
		}
	}


	/*
	template <class T>
	void Level::SaveEntitiesOfType(const std::string &name, TiXmlElement *element, Entity *fromEntity)
	{
		XMLFileNode xmlFileNode;

		const std::list<Entity*> *entities;
		if (fromEntity)
			entities = fromEntity->GetChildren();
		else
			entities = instance->scene->GetEntities();

		for (std::list<Entity*>::const_iterator i = entities->begin(); i != entities->end(); ++i)
		{
			Entity *entity = *i;
			T *t = dynamic_cast<T*>(entity);
			if (t)
			{
				TiXmlElement saveElement(name);
				xmlFileNode.element = &saveElement;

				instance->SaveEntities(&saveElement, entity);

				entity->Save(&xmlFileNode);

				element->InsertEndChild(saveElement);
			}

		}
	}
	*/

	/*
	template <class T>
	void Level::LoadEntitiesOfType(const std::string &name, TiXmlElement *element, Entity *intoEntity)
	{
		XMLFileNode xmlFileNode;

		TiXmlElement *eEntity = element->FirstChildElement(name);
		while (eEntity)
		{
			T *t = new T();
			Entity *entity = dynamic_cast<Entity*>(t);
			if (intoEntity == NULL)
				instance->scene->Add(entity);
			else
				intoEntity->Add(entity);

			instance->LoadEntities(eEntity, entity);

			xmlFileNode.element = eEntity;
			entity->Load(&xmlFileNode);

			eEntity = eEntity->NextSiblingElement(name);
		}
	}
	*/

	void Level::SaveEntities(TiXmlElement *element, Entity *fromEntity)
	{
		SaveEntitiesOfType<PathMesh>("PathMesh", element, fromEntity);
		SaveEntitiesOfType<FringeTile>("FringeTile", element, fromEntity);
		//SaveEntitiesOfType<Node>("Node", element, fromEntity);
	}

	void Level::LoadEntities(TiXmlElement *element, Entity *intoEntity)
	{
		if (element)
		{
			LoadEntitiesOfType<PathMesh>("PathMesh", element, intoEntity);
			LoadEntitiesOfType<FringeTile>("FringeTile", element, intoEntity);
			//LoadEntitiesOfType<Node>("Node", element, intoEntity);
		}
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
				Color backgroundColor = Graphics::GetBackgroundColor();
				if (backgroundColor != Color::black)
				{
					XMLWriteColor(&eLevel, "backgroundColor", backgroundColor);
				}

				XMLFileNode xmlFileNode(&eLevel);
				instance->scene->SaveLevel(&xmlFileNode);

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
				instance->scene->SaveEntities(&eEntities);
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
