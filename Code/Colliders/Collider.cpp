#include "Collider.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"

namespace Monocle
{
	bool Collider::Collide(Collider* a, Collider* b)
	{
		ColliderType typeA = a->GetColliderType();
		ColliderType typeB = b->GetColliderType();

		if (typeA == CT_RECT && typeB == CT_RECT)
			return CollideRectRect((RectangleCollider*)a, (RectangleCollider*)b);
	}

	bool Collider::CollideRectRect(RectangleCollider* a, RectangleCollider* b)
	{
		if (a->position.y + a->height < b->position.y)
			return false;

		if (a->position.y > b->position.y + b->height)
			return false;

		if (a->position.x + a->width < b->position.x)
			return false;

		if (a->position.x > b->position.x + b->width)
			return false;

		return true;
	}

	bool Collider::CollideCircleCircle(CircleCollider* a, CircleCollider* b)
	{
		Vector2 diff = b->position - a->position;
		return (diff.IsInRange(a->radius + b->radius));
	}

	bool Collider::CollideRectCircle(RectangleCollider* a, CircleCollider* b)
	{
		//The center of the circle is within the rectangle
		if (a->IntersectsPoint(b->position))
			return true;

		//Check the circle against the four edges of the rectangle
		Vector2 pB = Vector2(a->position.x + a->width, a->position.y);
		Vector2 pC = Vector2(a->position.x + a->width, a->position.y + a->height);
		Vector2 pD = Vector2(a->position.x, a->position.y + a->height);
		if (b->IntersectsLine(a->position, pB) || b->IntersectsLine(pB, pC) || b->IntersectsLine(pC, pD) || b->IntersectsLine(pD, a->position))
			return true;

		return false;
	}
}