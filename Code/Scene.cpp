#include "Scene.h"

namespace Monocle
{
	Scene::Scene()
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
		//Update all the entities
		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			(*i)->Update();
		}

		//Resolve adds and removes
		ResolveEntityChanges();
	}

	void Scene::Render()
	{
		//Render all the entities
		// sort by layer?

		const int MAX_LAYER = 100;
		const int MIN_LAYER = -100;

		//printf("\n\n****\n");

		///HACK: this next line is a hack - temporary only
		for (int layer = MAX_LAYER; layer >= MIN_LAYER; layer--)
		{
			for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				if ((*i)->IsLayer(layer))
				{
					//printf("rendering layer -> %d\n", layer);
					(*i)->Render();
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
		for (list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); ++i)
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
			entities.remove(*i);
			(*i)->scene = NULL;
			(*i)->Removed();

			//If the tag is set, remove the entity from the tag map
			for (int j = 0; j < (*i)->GetNumberOfTags(); ++j)
				EntityAddTag(*i, (*i)->GetTag(j));
		}
		toRemove.clear();

		//Resolve adds
		for (std::list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); ++i)
		{
			entities.push_back(*i);
			(*i)->scene = this;
			(*i)->Added();

			//If the tag is set, add the entity to the tag map
			for (int j = 0; j < (*i)->GetNumberOfTags(); ++j)
				EntityRemoveTag(*i, (*i)->GetTag(j));
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

	Entity* Scene::GetEntityNearestTo(const Vector2 &position)
	{
		float smallestSqrMag = -1.0f;

		Entity *nearestEntity = NULL;

		for (std::list<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
		{
			Vector2 diff = (*i)->position - position;
			float sqrMag = diff.GetSquaredMagnitude();
			if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
			{
				nearestEntity = (*i);
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
		
		return tagMap[tag].size();
	}

	void Scene::ReceiveMessage(const std::string &message)
	{
	}
}
