#pragma once

#include "Collider.h"
#include "../Vector2.h"

namespace Monocle
{
	// Axis-Aligned Bounding Box
	class RectangleCollider : public Collider
	{
	public:
		float width;
		float height;
		RectangleCollider(float width, float height, Vector2 offset);
		ColliderType GetColliderType();
		bool IntersectsPoint(const Vector2& point);
		bool IntersectsLine(const Vector2& start, const Vector2& end);
	};
}