#include "Transform.h"

namespace Monocle
{
	Transform::Transform()
		: position(Vector2::zero), rotation(0.0f), scale(Vector2::one)
	{
	}

	Transform::Transform(const Transform &transform)
		: position(transform.position), rotation(transform.rotation), scale(transform.scale)
	{

	}
}