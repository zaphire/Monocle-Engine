#include "Collider.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "../Entity.h"
#include <stdio.h> // for NULL

namespace Monocle
{
	Collider::Collider()
		: entity(NULL)
	{
	}

	void Collider::SetEntity(Entity* entity)
	{
		this->entity = entity;
	}

	Entity* Collider::GetEntity()
	{
		return entity;
	}

	Vector2 Collider::GetEntityPosition()
	{
		if (entity != NULL)
			return entity->position;
		else
			return Vector2();
	}

	bool Collider::Collide(Collider* a, Collider* b)
	{
		ColliderType typeA = a->GetColliderType();
		ColliderType typeB = b->GetColliderType();

		if (typeA == CT_RECT && typeB == CT_RECT)
			return CollideRectRect((RectangleCollider*)a, (RectangleCollider*)b);
		else if (typeA == CT_CIRCLE && typeB == CT_CIRCLE)
			return CollideCircleCircle((CircleCollider*)a, (CircleCollider*)b);
		else if (typeA == CT_RECT && typeB == CT_CIRCLE)
			return CollideRectCircle((RectangleCollider*)a, (CircleCollider*)b);
		else if (typeA == CT_CIRCLE && typeB == CT_RECT)
			return CollideRectCircle((RectangleCollider*)b, (CircleCollider*)a);

		// Unhandled case
		return false;
	}

	bool Collider::CollideRectRect(RectangleCollider* a, RectangleCollider* b)
	{
		Vector2 aOff = a->GetEntityPosition();
		Vector2 bOff = b->GetEntityPosition();

		if (a->offset.y + aOff.y + a->height*0.5f < b->offset.y + bOff.y - b->height*0.5f)
			return false;

		if (a->offset.y + aOff.y - a->height*0.5f > b->offset.y + bOff.y + b->height*0.5f)
			return false;

		if (a->offset.x + aOff.x + a->width*0.5f < b->offset.x + bOff.x - b->width*0.5f)
			return false;

		if (a->offset.x + aOff.x - a->width*0.5f > b->offset.x + bOff.x + b->width*0.5f)
			return false;

		return true;
	}

	bool Collider::CollideCircleCircle(CircleCollider* a, CircleCollider* b)
	{
		Vector2 aOff = a->GetEntityPosition();
		Vector2 bOff = b->GetEntityPosition();

		Vector2 diff = (b->offset + bOff) - (a->offset + aOff);
		return (diff.IsInRange(a->radius + b->radius));
	}

	bool Collider::CollideRectCircle(RectangleCollider* a, CircleCollider* b)
	{
		Vector2 aOff = a->GetEntityPosition();
		Vector2 bOff = b->GetEntityPosition();

		//The center of the circle is within the rectangle
		if (a->IntersectsPoint(bOff + b->offset))
			return true;

		//Check the circle against the four edges of the rectangle
		Vector2 pA = aOff + a->offset;
		Vector2 pB = aOff + Vector2(a->offset.x + a->width, a->offset.y);
		Vector2 pC = aOff + Vector2(a->offset.x + a->width, a->offset.y + a->height);
		Vector2 pD = aOff + Vector2(a->offset.x, a->offset.y + a->height);
		if (b->IntersectsLine(pA, pB) || b->IntersectsLine(pB, pC) || b->IntersectsLine(pC, pD) || b->IntersectsLine(pD, pA))
			return true;

		return false;
	}
}
