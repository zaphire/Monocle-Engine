#include "CircleCollider.h"
#include "../CollisionData.h"
#include <cmath>

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

	bool CircleCollider::IntersectsPoint(const Vector2& point, CollisionData *collisionData)
	{
		Vector2 ePos = GetEntityPosition();

		Vector2 diff = point - (ePos + offset);
		return (diff.IsInRange(radius));
	}

	bool CircleCollider::IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius, CollisionData *collisionData)
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

		bool didCollide = d.GetSquaredMagnitude() <= (radius + lineRadius) * (radius + lineRadius);

		if (didCollide && collisionData)
		{
			collisionData->hitPoint = closest;
			collisionData->penetration = fabs(d.GetMagnitude() - (radius + lineRadius));
			collisionData->normal = (ePos - closest).GetNormalized();
		}

		return didCollide;
	}

	float CircleCollider::GetCenterX(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.x;
		else
			return GetEntityPosition().x + offset.x;
	}

	float CircleCollider::GetCenterY(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.y;
		else
			return GetEntityPosition().y + offset.y;
	}

	Vector2 CircleCollider::GetCenter(bool relativeToEntity)
	{
		return Vector2(GetCenterX(relativeToEntity), GetCenterY(relativeToEntity));
	}
}