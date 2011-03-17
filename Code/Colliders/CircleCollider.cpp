#include "CircleCollider.h"

namespace Monocle
{
	CircleCollider::CircleCollider(float radius, Vector2 offset)
		: Collider()
	{
		this->offset	= offset;
		this->radius	= radius;
	}

	ColliderType CircleCollider::GetColliderType()
	{
		return CT_CIRCLE;
	}

	bool CircleCollider::IntersectsPoint(const Vector2& point)
	{
		Vector2 ePos = GetEntityPosition();

		Vector2 diff = point - (ePos + offset);
		return (diff.IsInRange(radius));
	}

	bool CircleCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
		//Algorithm stolen from: http://www.gamedev.net/topic/304578-finite-line-circle-intersection/page__view__findpost__p__2938618

		Vector2 ePos = GetEntityPosition();

		Vector2 dir = end - start;
		Vector2 diff = (ePos + offset) - start;
		float t = diff.Dot(dir) / dir.GetSquaredMagnitude();
		if (t < 0.0f)
			t = 0.0f;
		if (t > 1.0f)
			t = 1.0f;
		Vector2 closest = (t * dir) + start;
		Vector2 d = (ePos + offset) - closest;
		return d.GetSquaredMagnitude() <= radius * radius;
	}
}