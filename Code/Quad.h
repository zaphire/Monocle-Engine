#pragma once

#include "Vector2.h"


namespace Monocle
{
	class Quad
	{
	public:
        Vector2 tl, br;
        
		Quad();
		Quad(const Vector2& tl, const Vector2& br);
		Quad(const Quad& other);
	};
}