#pragma once

#include "Colliders/Collider.h"
#include "Colliders/RectangleCollider.h"
#include "Colliders/CircleCollider.h"

#include <string>
#include <vector>
#include <list>

namespace Monocle
{
	class Entity;

	class Collision
	{
	public:
		Collision();
		void Init();
		static CircleCollider* AddCircleCollider(Entity *entity, float radius, Vector2 offset);
		static RectangleCollider* AddRectangleCollider(Entity *entity, float width, float height, Vector2 offset);
		static void RemoveCollider(Collider *collider);

		static Collider* Collide(Entity *entity, const std::string &tag);
		std::vector<Collider*> Overlap(Entity *entity, const std::string &tag);

	private:
		static void RegisterColliderWithEntity(Collider *collider, Entity *entity);

		static Collision *instance;
		std::list<Collider*> colliders;
	};
}
