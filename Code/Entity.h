#pragma once

#include "Vector2.h"

namespace Monocle
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		Vector2 position;
		int layer;
		float depth;

		//Called by the scene when the entity should update its game logic
		virtual void Update();

		//Called by the scene when the entity should render
		virtual void Render();

		//Called by the scene when the entity is added to that scene
		virtual void Added();

		//Called by the scene when the entity is removed from that scene
		virtual void Removed();
	};
}