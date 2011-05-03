#pragma once

#include "Vector2.h"
#include "Colliders/Collider.h"

namespace Monocle
{
	//! Describes the result of a collision. Used to resolve collisions.
	class CollisionData
	{
	public:
		CollisionData();

		//! position where the collision occurred
		Vector2 hitPoint;
		//! normal of the collision
		Vector2 normal;
		//! how far into the collider we've moved
		float penetration;
		//! pointer to the collider that was hit
		Collider *collider;
	};
}