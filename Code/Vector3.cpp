#include "Vector3.h"

namespace Monocle
{
	Vector3 Vector3::zero = Vector3(0,0,0);
	Vector3 Vector3::one = Vector3(1,1,1);
	Vector3 Vector3::down = Vector3(0, -1, 0);
	Vector3 Vector3::up = Vector3(0, 1, 0);
	Vector3 Vector3::left = Vector3(-1, 0, 0);
	Vector3 Vector3::right = Vector3(1, 0, 0);

	Vector3::Vector3()
		: x(0.0f), y(0.0f), z(0.0f)
	{

	}

	Vector3::Vector3(float x, float y, float z)
		: x(x), y(y), z(z)
	{
	}
}
