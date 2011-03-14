#pragma once

#include <list>

using std::list;

namespace Monocle
{
	class Entity
	{
	public:
		void update();
		void render();
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		//Called by the main game loop every time the scene should update game logic
		void update();

		//Called by the main game loop every time the scene should render
		void render();

		//Remove and delete all entities in the scene
		void removeAll();

	private:
		//Holds all the entities currently in the scene
		list<Entity*>* entities;
	};
}