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
		//static CircleCollider* AddCircleCollider(Entity *entity, float radius, Vector2 offset=Vector2::zero);
		//static RectangleCollider* AddRectangleCollider(Entity *entity, float width, float height, Vector2 offset=Vector2::zero);

		static Collider* Collide(Entity *entity, const std::string &tag);

	private:
		friend class Entity;
		static void Collision::RegisterColliderWithEntity(Collider *collider, Entity *entity);
		static void Collision::RemoveCollider(Collider *collider);

	private:
		static Collision *instance;
		std::list<Collider*> colliders;
	};
}
