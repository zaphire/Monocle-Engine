#pragma once

namespace Monocle
{
	class Random
	{
	public:
		Random();
		static void Seed(unsigned int seed);
		static int Range(int start, int end);
		static float Percent();
		static float RangeFloat(float start, float end);
	private:
		//static Random *instance;
	};
}