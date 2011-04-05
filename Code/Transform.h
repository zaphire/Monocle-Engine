#pragma once

#include "Vector2.h"

namespace Monocle
{
	class Transform
	{
	public:
		Transform();
		Transform(const Transform &transform);

		Vector2 position;
		float rotation;
		Vector2 scale;
	};
}