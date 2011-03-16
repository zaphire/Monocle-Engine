#pragma once

#include "../Vector2.h"

namespace Monocle
{
	enum ColliderType
	{
		CT_RECT=0,
		CT_CIRCLE
	};

	class RectangleCollider;
	class CircleCollider;

	class Collider
	{
	public:
		Vector2 position;

		virtual ColliderType GetColliderType() = 0;
		virtual bool IntersectsPoint(const Vector2& point)=0;
		virtual bool IntersectsLine(const Vector2& start, const Vector2& end)=0;

		static bool Collide(Collider* a, Collider* b);
	private:
		static bool CollideRectRect(RectangleCollider* a, RectangleCollider* b);
		static bool CollideCircleCircle(CircleCollider* a, CircleCollider* b);
		static bool CollideRectCircle(RectangleCollider* a, CircleCollider* b);
	};
}