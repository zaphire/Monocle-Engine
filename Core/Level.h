#pragma once

#include <string>
#include <vector>
#include <list>

#include "Graphics.h"
#include "Tileset.h"
#include "Graphics/Tilemap.h"
#include "LevelEditor/FringeTile.h"

// temp, replace with generic file interface later
#include <TinyXML/tinyxml.h>
#include <XML/XMLFileNode.h>

namespace Monocle
{
	class Scene;

	class EntityType
	{
	public:
		EntityType();

		std::string name;
		std::string image;

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);
	};

	//!
	//! \brief Manages the saving and loading of Entity instances as well as other objects.
	//!
	//! Can be extended to save/load any derivation of Entity, by overriding a few functions.
	//!
	class Level
	{
	public:
		Level();
		void Init();

		static void SetScene(Scene *scene);
		static void LoadProject(const std::string &filename);
		static void Load(const std::string &filename, Scene* scene=NULL);
		static void Save();
		static void SaveAs(const std::string &filename);
		static void End(); // called on scene end

		static FringeTileset* GetCurrentFringeTileset();

		std::string name;

		std::list<Tilemap*> tilemaps;
		std::list<Tileset> tilesets;

		std::list<FringeTileset> fringeTilesets;

		int width, height;

		template <class T> static void SaveEntitiesOfType(const std::string &name, TiXmlElement *element, Entity *fromEntity=NULL)
		{
			XMLFileNode xmlFileNode;

			const std::list<Entity*> *entities;
			//if (fromEntity)
			//	entities = fromEntity->GetChildren();
			//else
			entities = instance->scene->GetEntities();

			for (std::list<Entity*>::const_iterator i = entities->begin(); i != entities->end(); ++i)
			{
				Entity *entity = *i;
				T *t = dynamic_cast<T*>(entity);
				if (t)
				{
					TiXmlElement saveElement(name);
					xmlFileNode.element = &saveElement;

					//instance->SaveEntities(&saveElement, entity);

					entity->Save(&xmlFileNode);

					element->InsertEndChild(saveElement);
				}

			}
		}

		template <class T> static void LoadEntitiesOfType(const std::string &name, TiXmlElement *element, Entity *intoEntity=NULL)
		{
			XMLFileNode xmlFileNode;

			TiXmlElement *eEntity = element->FirstChildElement(name);
			while (eEntity)
			{
				T *t = new T();
				Entity *entity = dynamic_cast<Entity*>(t);
				//if (intoEntity == NULL)
				instance->scene->Add(entity);
				entity->SetParent(intoEntity);
				//else
				//intoEntity->Add(entity);

				//instance->LoadEntities(eEntity, entity);

				xmlFileNode.element = eEntity;
				entity->Load(&xmlFileNode);

				eEntity = eEntity->NextSiblingElement(name);
			}
		}

	private:
		Tileset *GetTilesetByName(const std::string &name);
		FringeTileset *GetFringeTilesetByName(const std::string &name);

		Scene *scene;
		static Level *instance;
		std::string filename;

		FringeTileset *fringeTileset;

		std::list<EntityType> entityTypes;

		void SaveEntities(TiXmlElement *element, Entity *fromEntity=NULL);
		void LoadEntities(TiXmlElement *element, Entity *intoEntity=NULL);


	};
}
