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
		for (list<Entity*>::iterator i = entities.begin(); i != entities.end(); i++)
		{
			(*i)->Update();
		}

		//Resolve adds and removes
		ResolveEntityChanges();
	}

	void Scene::Render()
	{
		//Render all the entities
		for (list<Entity*>::iterator i = entities.begin(); i != entities.end(); i++)
		{
			(*i)->Render();
		}
	}

	void Scene::Add(Entity* entity)
	{
#ifdef DEBUG
		//Error: If the entity is already in the scene
		for (list<Entity*>::iterator i = entities.begin(); i != entities.end(); i++)
		{
			if ((*i) == entity)
			{
				Debug::Log("Adding an entity to the scene that is already in the scene.");
				break;
			}
		}

		//Error: If the entity is already marked to be added
		for (list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); i++)
		{
			if ((*i) == entity)
			{
				Debug::Log("Adding an entity to the scene that is already flagged to be added at the end of the frame.");
				break;
			}
		}
#endif

		toAdd.push_back(entity);
	}

	void Scene::Remove(Entity* entity)
	{
#ifdef DEBUG
		//Error: If the entity isn't in the entity list
		bool in = false;
		for (list<Entity*>::iterator i = entities.begin(); i != entities.end(); i++)
		{
			if ((*i) == entity)
			{
				in = true;
				break;
			}
		}
		if (!in)
			Debug::Log("Removing an entity from the scene that isn't in the scene.");

		//Error: If the entity is already marked to be removed
		for (list<Entity*>::iterator i = toRemove.begin(); i != toRemove.end(); i++)
		{
			if ((*i) == entity)
			{
				Debug::Log("Removing an entity from the scene that is already flagged for removal at the end of the frame.");
				break;
			}
		}
#endif

		toRemove.push_back(entity);
	}

	void Scene::RemoveAll()
	{
		toRemove.clear();
		for (list<Entity*>::iterator i = entities.begin(); i != entities.end(); i++)
			toRemove.push_back(*i);
	}

	void Scene::ResolveEntityChanges()
	{
		//Resolve removes
		for (list<Entity*>::iterator i = toRemove.begin(); i != toRemove.end(); i++)
		{
			entities.remove(*i);
			(*i)->Removed();
		}
		toRemove.clear();

		//Resolve adds
		for (list<Entity*>::iterator i = toAdd.begin(); i != toAdd.end(); i++)
		{
			entities.push_back(*i);
			(*i)->Added();
		}
		toAdd.clear();

		//Sort the entity list based on layer
	}
}

