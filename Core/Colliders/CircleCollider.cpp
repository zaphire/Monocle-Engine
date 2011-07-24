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

	float CircleCollider::GetRadius()
	{
		float scale = (fabs(GetEntity()->scale.x) + fabs(GetEntity()->scale.y)) * 0.5f;
		//printf("GetRadius scale: %f\n", scale);
		return radius * scale;
	}

	bool CircleCollider::IntersectsPoint(const Vector2& point, CollisionData *collisionData)
	{
		Vector2 ePos = GetEntity()->GetWorldPosition(offset);

		Vector2 diff = point - ePos;
		return (diff.IsInRange(GetRadius()));
	}

	bool CircleCollider::IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius, CollisionData *collisionData)
	{
		//Algorithm stolen from: http://www.gamedev.net/topic/304578-finite-line-circle-intersection/page__view__findpost__p__2938618

		Vector2 ePos = GetEntityPosition();
		Vector2 eWorldPos = GetEntity()->GetWorldPosition(offset);
		float eRadius = GetRadius();
		//float eRadius = radius;

		Vector2 dir = end - start;
		Vector2 diff = (eWorldPos) - start;

		float t = diff.Dot(dir) / dir.GetSquaredMagnitude();
		if (t < 0.0f)
			t = 0.0f;
		if (t > 1.0f)
			t = 1.0f;

		Vector2 closest = (t * dir) + start;
		Vector2 d = (eWorldPos) - closest;

		bool didCollide = d.IsInRange(eRadius + lineRadius);

		if (didCollide && collisionData)
		{
			Vector2 diff = start - ePos;
			diff.Clamp(eRadius);
			collisionData->hitPoint = ePos + diff;
			collisionData->penetration = fabs(d.GetMagnitude() - (eRadius + lineRadius));
			collisionData->normal = (ePos - closest).GetNormalized();
			collisionData->collider = this;
		}

		return didCollide;
	}

	float CircleCollider::GetCenterX(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.x;
		else
			return GetEntity()->GetWorldPosition(offset).x;
	}

	float CircleCollider::GetCenterY(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.y;
		else
			return GetEntity()->GetWorldPosition(offset).y;
	}

	Vector2 CircleCollider::GetCenter(bool relativeToEntity)
	{
		return Vector2(GetCenterX(relativeToEntity), GetCenterY(relativeToEntity));
	}
}