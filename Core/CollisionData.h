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

		//! Position where the collision occurred
		Vector2 hitPoint;
		//! Normal of the collision
		Vector2 normal;
		//! How far into the collider we've moved
		float penetration;
		//! Pointer to the collider that was hit
		Collider *collider;
	};
}
