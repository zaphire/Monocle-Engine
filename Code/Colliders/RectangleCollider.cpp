#include "RectangleCollider.h"

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

	bool RectangleCollider::IntersectsPoint(const Vector2& point)
	{
		Vector2 ePos = GetEntityPosition();

		return (point.x > ePos.x + offset.x - width*0.5f && point.y > ePos.y + offset.y - height*0.5f && point.x < ePos.x + offset.x + width*0.5f && point.y < ePos.y + offset.y + height*0.5f);
	}

	bool RectangleCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
		if (IntersectsPoint(start) || IntersectsPoint(end))
			return true;

		Vector2 ePos = GetEntityPosition();
		Vector2 pA = Vector2(ePos.x + offset.x - width*0.5f, ePos.y + offset.y - height*0.5f);
		Vector2 pB = Vector2(ePos.x + offset.x + width*0.5f, ePos.y + offset.y - height*0.5f);
		Vector2 pC = Vector2(ePos.x + offset.x + width*0.5f, ePos.y + offset.y + height*0.5f);
		Vector2 pD = Vector2(ePos.x + offset.x - width*0.5f, ePos.y + offset.y + height*0.5f);
		
		return (LinesIntersect(start, end, pA, pB) || LinesIntersect(start, end, pB, pC) || LinesIntersect(start, end, pC, pD) || LinesIntersect(start, end, pD, pA));
	}
}