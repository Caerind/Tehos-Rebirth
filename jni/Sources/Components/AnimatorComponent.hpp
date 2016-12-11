#ifndef KE_ANIMATIONCOMPONENT_HPP
#define KE_ANIMATIONCOMPONENT_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include "../System/Animation.hpp"

#include "../Core/SceneComponent.hpp"

namespace ke
{

class AnimatorComponent : public SceneComponent
{
	public:
		typedef std::shared_ptr<AnimatorComponent> Ptr;

		TYPE(AnimatorComponent)

		AnimatorComponent(Actor& actor);
		virtual ~AnimatorComponent();

		virtual bool updatable() const;
		virtual bool renderable() const;

		std::size_t getAnimationCount() const;

		void addAnimation(const std::string& name, const std::string& animationName);

		void removeAnimation(std::string const& name);

		void removeAllAnimations();

		bool isPlaying() const;
		void playAnimation(std::string const& name);
		void stopAnimation();

		sf::Time getElapsedTime() const;
		void setElapsedTime(sf::Time elapsed);

		Animation& getActualAnimation();
		Animation::Frame& getActualFrame();

		void update(sf::Time dt);
		
		sf::FloatRect getLocalBounds();
		sf::FloatRect getGlobalBounds();

		// TODO : Serialize color or remove it
		void setColor(const sf::Color& color);
		const sf::Color& getColor() const;

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
		virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states);

	private:
		bool mPlaying;
		sf::Sprite mSprite;
		std::unordered_map<std::string, Animation> mAnimations;
		std::string mActualTexture;
		std::string mActualAnimation;
		std::size_t mActualFrame;
		sf::Time mTimeElapsed;
};

} // namespace ke

#endif // KE_ANIMATIORCOMPONENT_HPP
