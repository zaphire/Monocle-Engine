#pragma once

namespace Monocle
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		//Called by the scene when the entity should update its game logic
		void Update();

		//Called by the scene when the entity should render
		void Render();

		//Called by the scene when the entity is added to that scene
		void Added();

		//Called by the scene when the entity is removed from that scene
		void Removed();
	};
}