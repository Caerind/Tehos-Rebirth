#ifndef KE_DISTRIBUTION_HPP
#define KE_DISTRIBUTION_HPP

#include <cassert>
#include <functional>
#include <type_traits>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Random.hpp"
#include "Vector2.hpp"

namespace ke
{

namespace priv
{
	template <typename T>
	struct Constant
	{
		explicit Constant(T value)
			: value(value)
		{
		}

		T operator()() const
		{
			return value;
		}

		T value;
	};
} // namespace priv

template <typename T>
class Distribution
{
	public:
        Distribution(T constant)
        : mFactory(priv::Constant<T>(constant))
		{
		}

        Distribution(std::function<T()> function)
		: mFactory(function)
		{
		}

		T operator()() const
		{
			return mFactory();
		}

	private:
		std::function<T()> mFactory;
};

namespace Distributions
{

Distribution<int> uniform(int min, int max);
Distribution<unsigned int> uniform(unsigned int min, unsigned int max);
Distribution<float> uniform(float min, float max);
Distribution<sf::Time> uniform(sf::Time min, sf::Time max);
Distribution<sf::Vector2f> rect(const sf::Vector2f& center, const sf::Vector2f& halfSize);
Distribution<sf::Vector2f> rect(const sf::FloatRect& rect);
Distribution<sf::Vector2f> circle(const sf::Vector2f& center, float radius);
Distribution<sf::Vector2f> deflect(const sf::Vector2f& direction, float maxRotation);
Distribution<sf::Vector2f> project(const sf::Vector2f& direction, float maxRotation, float minVel, float maxVel);
Distribution<sf::Color> colorGrade(const sf::Color& color, float min, float max);

template <typename T>
Distribution<T> uniformT(T min, T max)
{
	if (min > max)
	{
		T temp = min;
		max = min;
		min = temp;
	}
    return Distribution<T>([=] () -> T
    {
        return random(min, max);
    });
}

} // namespace Distributions

} // namespace ke

#endif // KE_DISTRIBUTION_HPP
