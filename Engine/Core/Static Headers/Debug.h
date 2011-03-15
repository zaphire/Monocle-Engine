#pragma once

#include "Vector2.h"
#include "Vector3.h"

namespace Monocle
{
	class Debug
	{
	public:
		void Init();
		static void Log(const char *string);
		static void Log(int num);
		static void Log(long num);
		static void Log(float num);
		static void Log(double num);
		static void Log(const Vector2& vec);
		static void Log(const Vector3& vec);
	};
}