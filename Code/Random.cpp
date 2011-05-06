#include "Random.h"
#include <time.h>
#include <stdlib.h>

namespace Monocle
{
	Random::Random()
	{
		srand(static_cast<unsigned int>(time(NULL)));
	}

	void Random::Seed(unsigned int seed)
	{
		srand(seed);
	}
	
	int Random::Range(int start, int end)
	{
		return start + (rand() % (end-start));
	}

  float Random::Unit()
  {
    return (float)rand()/(float)RAND_MAX;
  }
}