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

	class CollisionData
	{
	public:
		CollisionData();

		Vector2 hitPoint;
		Vector2 normal;
		Collider *collider;
	};

	class Collision
	{
	public:
		Collision();
		void Init();
		//static CircleCollider* AddCircleCollider(Entity *entity, float radius, Vector2 offset=Vector2::zero);
		//static RectangleCollider* AddRectangleCollider(Entity *entity, float width, float height, Vector2 offset=Vector2::zero);

		static Collider* Collide(Entity *entity, const std::string &tag, CollisionData *collisionData=NULL);

	private:
		friend class Entity;
		static void RegisterColliderWithEntity(Collider *collider, Entity *entity);
		static void RemoveCollider(Collider *collider);

	private:
		static Collision *instance;
		std::list<Collider*> colliders;
	};
}
