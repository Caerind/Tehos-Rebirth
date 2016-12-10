#include "Random.hpp"

namespace ke
{

Random Random::mRandom;

int random(int min, int max)
{
    return Random::instance()(min, max);
}

unsigned int random(unsigned int min, unsigned int max)
{
    return Random::instance()(min, max);
}

bool randomBool()
{
    return (Random::instance()(0, 1) == 1);
}

float random(float min, float max)
{
    return Random::instance()(min, max);
}

float randomDev(float middle, float deviation)
{
	if (deviation < 0.f)
    {
        deviation *= (-1.f);
    }
	return Random::instance()(middle - deviation, middle + deviation);
}

void setRandomSeed(const std::string& seed)
{
    Random::instance().setSeed(seed);
}

std::string getRandomSeed()
{
    return Random::instance().getSeed();
}

} // namespace ke