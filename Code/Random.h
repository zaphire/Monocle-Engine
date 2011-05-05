#pragma once

namespace Monocle
{
	class Random
	{
	public:
		Random();
		static void Seed(unsigned int seed);
		static int Range(int start, int end);
    static float Unit(); // Random float from 0.0f-1.0f
	private:
		//static Random *instance;
	};
}