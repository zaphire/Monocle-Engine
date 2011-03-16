#pragma once

#include "Collider.h"
#include "../Vector2.h"

namespace Monocle
{
	class CircleCollider : public Collider
	{
	public:
		float radius;
		CircleCollider(float radius, Vector2 offset);
		ColliderType GetColliderType();
		bool IntersectsPoint(const Vector2& point);
		bool IntersectsLine(const Vector2& start, const Vector2& end);
	};
}