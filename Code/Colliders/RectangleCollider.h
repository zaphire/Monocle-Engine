#pragma once

#include "Collider.h"
#include "../Vector2.h"

namespace Monocle
{
	class RectangleCollider : public Collider
	{
	public:
		float width;
		float height;
		RectangleCollider(Vector2 position, float width, float height);
		ColliderType GetColliderType();
		bool IntersectsPoint(const Vector2& point);
		bool IntersectsLine(const Vector2& start, const Vector2& end);
	};
}