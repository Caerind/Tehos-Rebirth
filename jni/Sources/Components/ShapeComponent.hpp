#ifndef KE_SHAPECOMPONENT_HPP
#define KE_SHAPECOMPONENT_HPP

#include <SFML/Graphics/ConvexShape.hpp>

#include "../Core/SceneComponent.hpp"

namespace ke
{

class ShapeComponent : public SceneComponent
{
	public:
		typedef std::shared_ptr<ShapeComponent> Ptr;

		TYPE(ShapeComponent)

		ShapeComponent(Actor& actor);
		virtual ~ShapeComponent();

		virtual bool renderable() const;

		void setPointCount(std::size_t points);
		std::size_t getPointCount() const;

		void setPoint(std::size_t index, sf::Vector2f const& point);
		sf::Vector2f getPoint(std::size_t index) const;

		void setOutlineThickness(float thickness);
		float getOutlineThickness() const;

		void setOutlineColor(sf::Color const& color);
		sf::Color getOutlineColor() const;

		void setFillColor(sf::Color const& color);
		sf::Color getFillColor() const;

		sf::FloatRect getLocalBounds();
		sf::FloatRect getGlobalBounds();

		void setPoints(const std::vector<sf::Vector2f>& points);
		std::vector<sf::Vector2f> getPoints() const;

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
		virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states);

	private:
		std::vector<sf::Vector2f> mPoints;
		sf::ConvexShape mShape;
};

} // namespace ke

#endif // KE_SHAPECOMPONENT_HPP
