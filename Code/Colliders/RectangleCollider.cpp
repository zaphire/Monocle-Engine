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
		return (point.x > offset.x - width*0.5f && point.y > offset.y - height*0.5f && point.x < offset.x + width*0.5f && point.y < offset.y + height*0.5f);
	}

	bool RectangleCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
		//TODO!
		return false;
	}
}