#pragma once

#include "Vector2.h"
#include "Colliders/Collider.h"

namespace Monocle
{
	class CollisionData
	{
	public:
		CollisionData();

		Vector2 hitPoint;
		Vector2 normal;
		Collider *collider;
	};
}