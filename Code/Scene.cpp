#include "Scene.h"

namespace Monocle
{
	Scene::Scene()
		: isVisible(true), isPaused(false)
	{

	}

	Scene::~Scene()
	{
		RemoveAll();
		ResolveEntityChanges();
	}

	void Scene::Begin()
	{

	}

	void Scene::End()
	{

	}

	void Scene::Update()
	{
		if (!isPaused)
		{
			//Update all the entities
			for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				(*i)->Update();
			}
		}

		//Resolve adds and removes
		ResolveEntityChanges();
	}

	void Scene::Render()
	{
		if (isVisible)
		{
			//Render all the entities
			// sort by layer?

			const int MAX_LAYER = 100;
			const int MIN_LAYER = -100;

			//printf("\n\n****\n");

			///HACK: this next line is a hack - temporary only
			// TODO sort entities into layer buckets? or one big sorted list?
			for (int layer = MAX_LAYER; layer >= MIN_LAYER; layer--)
			{
				for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
				{
					if ((*i)->IsLayer(layer) && (*i)->isVisible)
					{
						(*i)->Render();
					}
				}
			}
		}
	}

	void Scene::Add(Entity* entity)
	{
#ifdef DEBUG
		//Error: If the entity is already in the scene
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i) == entity)
			{
				Debug::Log("ERROR: Adding an entity to the scene that is already in the scene.");
				break;
			}
		}

		//Error: If the entity is already marked to be added
		for (std::list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); ++i)
		{
			if ((*i) == entity)
			{
				Debug::Log("ERROR: Adding an entity to the scene that is already flagged to be added at the end of the frame.");
				break;
			}
		}
#endif
		if (entity->scene != NULL)
		{
			Debug::Log("ERROR: Trying to add an Entity to a Scene, but Entity is already in a Scene");
		}
		else
		{
			toAdd.push_back(entity);
		}
	}

	void Scene::Remove(Entity* entity)
	{
#ifdef DEBUG
		//Error: If the entity isn't in the entity list
		bool in = false;
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i) == entity)
			{
				in = true;
				break;
			}
		}
		if (!in)
			Debug::Log("ERROR: Removing an entity from the scene that isn't in the scene.");

		//Error: If the entity is already marked to be removed
		for (std::list<Entity*>::iterator i = toRemove.begin(); i != toRemove.end(); ++i)
		{
			if ((*i) == entity)
			{
				Debug::Log("ERROR: Removing an entity from the scene that is already flagged for removal at the end of the frame.");
				break;
			}
		}
#endif

		toRemove.push_back(entity);
	}

	void Scene::RemoveAll()
	{
		toRemove.clear();
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			toRemove.push_back(*i);
	}

	void Scene::ResolveEntityChanges()
	{
		//Resolve removes
		for (std::list<Entity*>::iterator i = toRemove.begin(); i != toRemove.end(); ++i)
		{
			(*i)->Destroy();

			entities.remove(*i);

			//If the tag is set, remove the entity from the tag map
			for (int j = 0; j < (*i)->GetNumberOfTags(); ++j)
				EntityRemoveTag(*i, (*i)->GetTag(j));

			(*i)->scene = NULL;
			(*i)->Removed();

			delete (*i);
		}
		toRemove.clear();

		//Resolve adds
		for (std::list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); ++i)
		{
			entities.push_back(*i);

			//If the tag is set, add the entity to the tag map
			for (int j = 0; j < (*i)->GetNumberOfTags(); ++j)
				EntityAddTag(*i, (*i)->GetTag(j));

			(*i)->scene = this;
			(*i)->Added();
		}
		toAdd.clear();

		//TODO: Sort the entity list based on layer
	}

	void Scene::EntityAddTag(Entity* entity, const std::string& tag)
	{
		tagMap[tag].push_back(entity);
	}

	void Scene::EntityRemoveTag(Entity* entity, const std::string& tag)
	{
		tagMap[tag].remove(entity);
	}

	Entity* Scene::GetEntity(int index)
	{
		int c = 0;
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if (c == index)
				return *i;
			c++;
		}
		return NULL;
	}

	Entity* Scene::GetNearestEntity(const Vector2 &position, Entity *ignoreEntity)
	{
		float smallestSqrMag = -1.0f;

		Entity *nearestEntity = NULL;

		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i) != ignoreEntity)
			{
				Vector2 diff = (*i)->position - position;
				float sqrMag = diff.GetSquaredMagnitude();
				if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
				{
					nearestEntity = (*i);
				}
			}
		}

		return nearestEntity;
	}

	/*
	Entity* Scene::GetNearestEntityContaining(const Vector2 &position, Entity *ignoreEntity)
	{
		float smallestSqrMag = -1.0f;

		Entity *nearestEntity = NULL;

		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i) != ignoreEntity)
			{
				if ((*i)->IsPositionInGraphic(position))
				{
					Vector2 diff = (*i)->position - position;
					float sqrMag = diff.GetSquaredMagnitude();
					if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
					{
						nearestEntity = (*i);
					}
				}
			}
		}

		return nearestEntity;
	}
	*/

	Entity* Scene::GetNearestEntityByControlPoint(const Vector2 &position, const std::string &tag, Entity *ignoreEntity)
	{
		float smallestSqrMag = -1.0f;

		Entity *nearestEntity = NULL;

		std::list<Entity*> *entities = &this->entities;

		if (tag != "")
		{
			entities = GetAllTag(tag);
		}

		if (entities != NULL)
		{
			for (std::list<Entity*>::iterator i = entities->begin(); i != entities->end(); ++i)
			{
				if ((*i) != ignoreEntity)
				{
					Vector2 diff = (*i)->position - position;
					if (diff.IsInRange(20))
					{
						Vector2 diff = (*i)->position - position;
						float sqrMag = diff.GetSquaredMagnitude();
						if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
						{
							nearestEntity = (*i);
						}
					}
				}
			}
		}


		return nearestEntity;
	}

	Entity* Scene::GetFirstTag(const std::string& tag)
	{
		if (tagMap.count(tag) == 0 || tagMap[tag].size() == 0)
			return NULL;
		
		return tagMap[tag].front();
	}

	std::list<Entity*>* Scene::GetAllTag(const std::string& tag)
	{
		if (tagMap.count(tag) == 0)
			return NULL;
		else
			return &tagMap[tag];
	}

	int Scene::GetAmountTag(const std::string& tag)
	{
		if (tagMap.count(tag) == 0) 
			return 0;
		
		return static_cast<int>(tagMap[tag].size());
	}

	void Scene::ReceiveNote(const std::string &note)
	{
	}

	void Scene::RelayNoteTo(const std::string &tag, const std::string &note)
	{
		std::list<Entity*>* taggedEntities = GetAllTag(tag);
		for (std::list<Entity*>::iterator i = (*taggedEntities).begin(); i != (*taggedEntities).end(); ++i)
		{
			//if ((*i) != fromEntity)
			(*i)->ReceiveNote(tag, note);//, fromEntity);
		}
	}

	Entity* Scene::GetEntityAtPosition(const Vector2 &position, SearchType searchType)
	{
		if (searchType == SEARCH_TOP)
		{
			for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				if ((*i)->IsPositionInGraphic(position))
				{
					return *i;
				}
			}
		}
		else if (searchType == SEARCH_RECURSIVE)
		{
			Debug::Log("recursive");
			for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				Entity *entity = (*i)->GetChildEntityAtPosition(position);
				if (entity)
				{
					return entity;
				}
			}
		}
		return NULL;
	}

	/*
	void Scene::SendNoteToGame(const std::string &note)
	{
		if (game)
		{
			game->ReceiveNote(note);
		}
	}
	*/
}
