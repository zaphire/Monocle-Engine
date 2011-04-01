#pragma once

#include "Collider.h"
#include "../Vector2.h"
#include "../Entity.h"

namespace Monocle
{
	class CircleCollider : public Collider
	{
	public:
		float radius;
		CircleCollider(float radius, Vector2 offset=Vector2::zero);
		ColliderType GetColliderType();
		bool IntersectsPoint(const Vector2& point);
		bool IntersectsLine(const Vector2& start, const Vector2& end);

		float GetCenterX(bool relativeToEntity = false);
		float GetCenterY(bool relativeToEntity = false);
		Vector2 GetCenter(bool relativeToEntity = false);
	};
}