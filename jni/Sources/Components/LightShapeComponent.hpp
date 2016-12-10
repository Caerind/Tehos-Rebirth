#ifndef KE_LIGHTSHAPECOMPONENT_HPP
#define KE_LIGHTSHAPECOMPONENT_HPP

#include "../ExtLibs/LetThereBeLight.hpp"

#include "../Core/SceneComponent.hpp"

namespace ke
{

class LightShapeComponent : public SceneComponent
{
	public:
		typedef std::shared_ptr<LightShapeComponent> Ptr;

		TYPE(LightShapeComponent)

		LightShapeComponent(Actor& actor);
		virtual ~LightShapeComponent();

		void onRegister();
		void onUnregister();

		void setPointCount(std::size_t points);
		std::size_t getPointCount() const;

		void setPoint(std::size_t index, sf::Vector2f const& point);
		sf::Vector2f getPoint(std::size_t index) const;

		sf::FloatRect getLocalBounds();
		sf::FloatRect getGlobalBounds();

		void setPoints(const std::vector<sf::Vector2f>& points);
		std::vector<sf::Vector2f> getPoints() const;

		void setOn(bool on);
		bool isOn() const;

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
		virtual void onTransformNotified();

	private:
		std::vector<sf::Vector2f> mPoints;
		ltbl::LightShape* mLightShape;
};

} // namespace ke

#endif // KE_LIGHTSHAPECOMPONENT_HPP
