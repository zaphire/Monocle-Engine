#include "RectangleCollider.h"

#include <cmath>

namespace Monocle
{
	//Note: collision is centered
	// so 0,0 is the center of the sprite
	// unless you set an offset (position)

	RectangleCollider::RectangleCollider(float width, float height, Vector2 offset)
		: Collider()
	{
		this->offset	= offset;
		this->width		= width;
		this->height	= height;
	}

	ColliderType RectangleCollider::GetColliderType()
	{
		return CT_RECT;
	}

	bool RectangleCollider::IntersectsPoint(const Vector2& point, CollisionData *collisionData)
	{
		//Vector2 ePos = GetEntityPosition();

		return (point.x > GetLeft() && point.y > GetTop() && point.x < GetRight() && point.y < GetBottom());
	}

	bool RectangleCollider::IntersectsLine(const Vector2& start, const Vector2& end, float lineRadius, CollisionData *collisionData)
	{
		//TODO: Support line width!

		if (!collisionData)
		{
			if (IntersectsPoint(start) || IntersectsPoint(end))
				return true;
		}

		Vector2 pA = GetTopLeft();
		Vector2 pB = GetTopRight();
		Vector2 pC = GetBottomRight();
		Vector2 pD = GetBottomLeft();
		
		return (LinesIntersect(start, end, pA, pB, collisionData) || LinesIntersect(start, end, pB, pC, collisionData) || LinesIntersect(start, end, pC, pD, collisionData) || LinesIntersect(start, end, pD, pA, collisionData));
	}

	float RectangleCollider::GetRight(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.x + width*0.5f * fabs(GetEntity()->scale.x);
		else
			return GetEntityPosition().x + offset.x + width*0.5f * fabs(GetEntity()->scale.x);
	}

	float RectangleCollider::GetLeft(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.x - width*0.5f * fabs(GetEntity()->scale.x);		
		else
			return GetEntityPosition().x + offset.x - width*0.5f * fabs(GetEntity()->scale.x);
	}

	float RectangleCollider::GetTop(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.y - height*0.5f * fabs(GetEntity()->scale.y);
		else
			return GetEntityPosition().y + offset.y - height*0.5f * fabs(GetEntity()->scale.y);	
	}

	float RectangleCollider::GetBottom(bool relativeToEntity)
	{
		if (relativeToEntity)
			return offset.y + height*0.5f * fabs(GetEntity()->scale.y);	
		else
			return GetEntityPosition().y + offset.y + height*0.5f * fabs(GetEntity()->scale.y);
	}

	Vector2 RectangleCollider::GetTopLeft(bool relativeToEntity)
	{
		return Vector2(GetLeft(relativeToEntity), GetTop(relativeToEntity));
	}

	Vector2 RectangleCollider::GetTopRight(bool relativeToEntity)
	{
		return Vector2(GetRight(relativeToEntity), GetTop(relativeToEntity));
	}

	Vector2 RectangleCollider::GetBottomLeft(bool relativeToEntity)
	{
		return Vector2(GetLeft(relativeToEntity), GetBottom(relativeToEntity));
	}

	Vector2 RectangleCollider::GetBottomRight(bool relativeToEntity)
	{
		return Vector2(GetRight(relativeToEntity), GetBottom(relativeToEntity));
	}
}