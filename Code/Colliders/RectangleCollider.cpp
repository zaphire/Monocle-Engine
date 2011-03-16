#include "RectangleCollider.h"

namespace Monocle
{
	RectangleCollider::RectangleCollider(Vector2 position, float width, float height)
	{
		this->position	= position;
		this->width		= width;
		this->height	= height;
	}

	ColliderType RectangleCollider::GetColliderType()
	{
		return CT_RECT;
	}

	bool RectangleCollider::IntersectsPoint(const Vector2& point)
	{
		return (point.x >= position.x && point.y >= position.y && point.x < position.x + width && point.y < position.y + height);
	}

	bool RectangleCollider::IntersectsLine(const Vector2& start, const Vector2& end)
	{
		//TODO!
		return false;
	}
}