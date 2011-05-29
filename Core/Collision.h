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
        //! Returns the first Collider with the specified tag that intersects with the line.
        //! \param start The beginning vector of the line to cast.
        //! \param end The end vector of the line to cast.
        //! \param radius The radius of the line to cast.
        //! \param tag The tag of the Colliders to be checked for intersection
        //! \param collisionData [out] The data resulting from the collision
		static Collider* LineCast(const Vector2 &start, const Vector2 &end, float radius, const std::string &tag, CollisionData *collisionData = NULL);

	private:
		friend class Entity;
		friend class Game;
		
		Collision();
		void Init();
		
		static void RegisterColliderWithEntity(Collider *collider, Entity *entity);
		static void RemoveCollider(Collider *collider);
		static Collider* Collide(Entity *entity, const std::string &tag, CollisionData *collisionData=NULL);

	private:
		static Collision *instance;
		std::list<Collider*> colliders;
	};
}
