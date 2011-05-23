#pragma once

#include "Colliders/Collider.h"
#include "Colliders/RectangleCollider.h"
#include "Colliders/CircleCollider.h"

#include "CollisionData.h"

#include <string>
#include <vector>
#include <list>

namespace Monocle
{
	class Entity;

	//!
	//! \brief Manages collision for Monocle. In most cases there is no need to reference this class, use the Entity::Collide function instead.
	//!
	//! Collision tracks Collider instances, and uses their Entity pointers to check for which entities fit the tag description passed to Collision::Collide.
	//! 
	class Collision
	{
	public:
		Collision();
		void Init();
		static Collider* LineCast(const Vector2 &start, const Vector2 &end, float radius, const std::string &tag, CollisionData *collisionData = NULL);

	private:
		friend class Entity;
		static void RegisterColliderWithEntity(Collider *collider, Entity *entity);
		static void RemoveCollider(Collider *collider);
		static Collider* Collide(Entity *entity, const std::string &tag, CollisionData *collisionData=NULL);

	private:
		static Collision *instance;
		std::list<Collider*> colliders;
	};
}
