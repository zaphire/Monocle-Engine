#include "Scene.h"

namespace Monocle
{
	Scene::Scene()
	{
		entities = new list<Entity*>();
	}

	Scene::~Scene()
	{
		removeAll();
		delete entities;
	}

	void Scene::update()
	{

	}

	void Scene::render()
	{

	}

	void Scene::removeAll()
	{
		for (int i = 0; i < entities->size(); i++)
			delete (&entities)[i];	
		entities->clear();
	}
}