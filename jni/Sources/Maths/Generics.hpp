#ifndef KE_GENERICS_HPP
#define KE_GENERICS_HPP

#include <algorithm>
#include <sstream>

namespace ke
{

template <typename T>
T clamp(T const& value, T const& min, T const& max)
{
    return std::max(min, std::min(value, max));
}

template <typename T>
T abs(T const& value)
{
    return std::abs(value);
}

template <typename T>
T sqrt(T const& value)
{
    return std::sqrt(value);
}

template <typename T>
std::string decToHex(T decimal)
{
    std::ostringstream oss;
    oss << std::hex << decimal;
    return oss.str();
}

template <typename T>
T hexToDec(std::string const& hexadecimal)
{
    std::istringstream iss(hexadecimal);
    T decimal;
    iss >> std::hex >> decimal;
    return decimal;
}

template <typename T>
std::string decToOct(T decimal)
{
    std::ostringstream oss;
    oss << std::oct << decimal;
    return oss.str();
}

template <typename T>
T octToDec(std::string const& octal)
{
    std::istringstream iss(octal);
    T decimal;
    iss >> std::oct >> decimal;
    return decimal;
}

template <typename T>
std::string decToBin(T decimal)
{
    std::vector<bool> values;
    while (decimal > 0)
    {
        values.push_back(((decimal % 2) == 1));
        decimal /= 2;
    }
    std::string r;
    for (std::size_t i = 0; i < values.size(); i++)
    {
        r += (values[i] == true) ? "1" : "0";
    }
    std::reverse(r.begin(), r.end());
    return r;
}

template <typename T>
T binToDec(std::string const& binary)
{
    T decimal = 0, factor = 1;
    std::string temp = binary;
    std::reverse(temp.begin(), temp.end());
    for (std::size_t i = 0; i < temp.size(); i++)
    {
        decimal += factor * ((temp[i] == '1') ? 1 : 0);
        factor *= 2;
    }
    return decimal;
}

class Average
{
	public:
		Average() : mAverage(0.f), mCount(0) {}

		float add(float term) 
		{ 
			mAverage = ((mAverage * mCount) + term) * 1.f / (mCount + 1);
			mCount++;
			return mAverage;
		}

		float get() { return mAverage; }

		std::size_t getCount() { return mCount; }

	private:
		float mAverage;
		std::size_t mCount;
};

} // namespace ke

#endif // KE_GENERICS_HPP
