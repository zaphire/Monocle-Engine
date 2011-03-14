#pragma once

#include <list>

using std::list;

namespace Monocle
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		void update();
		void render();
	private:
		list<Entity*> entities;
	};
}