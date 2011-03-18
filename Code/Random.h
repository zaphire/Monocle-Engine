#pragma once

namespace Monocle
{
	class Random
	{
	public:
		Random();
		static void Seed(unsigned int seed);
		static int Range(int start, int end);
	private:
		//static Random *instance;
	};
}