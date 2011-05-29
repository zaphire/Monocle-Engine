#pragma once

#include "Vector2.h"


namespace Monocle
{
	//!
	//! \brief Describes a rectangle.
	//!
	class Rect
	{
	public:
		Vector2 topLeft, bottomRight;

		Rect();
		Rect(const Vector2& topLeft, const Vector2& bottomRight);
		Rect(const Rect& other);
	};
}