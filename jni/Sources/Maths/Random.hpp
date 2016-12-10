#ifndef KE_RANDOM_HPP
#define KE_RANDOM_HPP

#include <cassert>
#include <chrono>
#include <random>
#include <sstream>

namespace ke
{

class Random
{
    public:
        Random()
        {
            std::ostringstream oss;
            oss << std::chrono::system_clock::now().time_since_epoch().count();
            setSeed(oss.str());
        }

        Random(const std::string& seed)
        {
            setSeed(seed);
        }

        int operator()(int min, int max)
        {
			if (min > max)
			{
				int temp = min;
				max = min;
				min = temp;
			}
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(mGenerator);
        }

        unsigned int operator()(unsigned int min, unsigned int max)
        {
			if (min > max)
			{
				unsigned int temp = min;
				max = min;
				min = temp;
			}
            std::uniform_int_distribution<unsigned int> distribution(min, max);
            return distribution(mGenerator);
        }

        float operator()(float min, float max)
        {
			if (min > max)
			{
				float temp = min;
				max = min;
				min = temp;
			}
            std::uniform_real_distribution<float> distribution(min, max);
            return distribution(mGenerator);
        }

        void setSeed(const std::string& seed)
        {
            mSeed = seed;
            std::seed_seq s(mSeed.begin(), mSeed.end());
            mGenerator.seed(s);
        }

        const std::string& getSeed() const
        {
            return mSeed;
        }

        static Random& instance()
        {
            return mRandom;
        }

    private:
        static Random mRandom;
        std::mt19937 mGenerator;
        std::string mSeed;
};

int random(int min, int max);

unsigned int random(unsigned int min, unsigned int max);

bool randomBool();

float random(float min, float max);

float randomDev(float middle, float deviation);

void setRandomSeed(const std::string& seed);

std::string getRandomSeed();

template <typename T>
class RandomTable
{
	public:
		RandomTable() : mTable(), mSum(0) {}

		void add(std::size_t percent, T const& value) { mTable.push_back(std::make_pair(percent, value)); mSum += percent; }

		T get()
		{
			int value = static_cast<int>(random(0, mSum));
			for (std::size_t i = 0; i < mTable.size(); i++)
			{
				value -= static_cast<int>(mTable[i].first);
				if (value <= 0)
				{
					return mTable[i].second;
				}
			}
		}

	private:
		std::vector<std::pair<std::size_t, T>> mTable;
		int mSum;
};

} // namespace ke

#endif // KE_RANDOM_HPP
