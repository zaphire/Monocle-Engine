#pragma once

namespace Monocle
{
	class Random
	{
	public:
		Random();
		static void Seed(unsigned int seed);
		//! not inclusive
		static int Int(int start, int end);
		static float Float(float start, float end);
		static float Percent();
		
	private:
		//static Random *instance;
	};
}