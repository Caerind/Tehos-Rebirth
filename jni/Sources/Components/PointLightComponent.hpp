#ifndef KE_POINTLIGHTCOMPONENT_HPP
#define KE_POINTLIGHTCOMPONENT_HPP

#include "../ExtLibs/LetThereBeLight.hpp"

#include "../Core/SceneComponent.hpp"

namespace ke
{

class PointLightComponent : public SceneComponent
{
	public:
		typedef std::shared_ptr<PointLightComponent> Ptr;

		TYPE(PointLightComponent)

		PointLightComponent(Actor& actor);
		virtual ~PointLightComponent();

		void onRegister();
		void onUnregister();

		void setTexture(const std::string& texture);
		std::string getTexture() const;

		void setOrigin(const sf::Vector2f& origin);
		void setOrigin(float x, float y);
		sf::Vector2f getOrigin() const;

		void setColor(sf::Color color);
		sf::Color getColor() const;

		void setIntensity(float intensity);
		float getIntensity() const;

		void setOn(bool on);
		bool isOn() const;

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	protected:
		void onTransformNotified();

	protected:
		ltbl::LightPointEmission* mLight;
		std::string mTexture;
};

} // namespace ke

#endif // KE_POINTLIGHTCOMPONENT_HPP