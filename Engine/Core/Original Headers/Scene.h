#pragma once

#include "Debug.h"
#include <list>
#include "Entity.h"

using std::list;

namespace Monocle
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		//Called when this scene becomes the currently active scene
		virtual void Begin();

		//Called when this scene is ended and a different one becomes the active scene
		virtual void End();

		//Called by the main game loop every time the scene should update game logic
		virtual void Update();

		//Called by the main game loop every time the scene should render
		void Render();

		//Add an entity to the scene
		void Add(Entity* entity);

		//Remove an entity from the scene
		void Remove(Entity* entity);

		//Remove all entities in the scene
		void RemoveAll();

	private:
		//Holds all the entities currently in the scene
		list<Entity*> entities;

		//All the entities that will be added at the end of the frame
		list<Entity*> toAdd;

		//All the entities that will be removed at the end of the frame
		list<Entity*> toRemove;

		//Resolves all entities to be added or removed
		void ResolveEntityChanges();
	};
}