#ifndef KE_PARTICLECOMPONENT_HPP
#define KE_PARTICLECOMPONENT_HPP

#include <array>
#include <functional>
#include <iostream>
#include <memory>

#include <SFML/Graphics/VertexArray.hpp>

#include "../Maths/Distribution.hpp"

#include "../Core/SceneComponent.hpp"

namespace ke
{

class ParticleComponent : public SceneComponent
{
    public:
		typedef std::shared_ptr<ParticleComponent> Ptr;

		TYPE(ParticleComponent)

		typedef std::array<sf::Vertex, 6> Quad;
		
		class Particle
		{
			public:
				Particle(sf::Time lifetime) : position(), velocity(), rotation(), rotationSpeed(), scale(1.f, 1.f), color(255, 255, 255), textureIndex(0), passedLifetime(sf::Time::Zero), totalLifetime(lifetime) {}

				sf::Vector2f position;
				sf::Vector2f velocity;
				float rotation;
				float rotationSpeed;
				sf::Vector2f scale;
				sf::Color color;
				std::size_t textureIndex;

				sf::Time passedLifetime;
				sf::Time totalLifetime;
		};

		typedef std::function<void(Particle&, sf::Time)> Affector;

	public:
		ParticleComponent(Actor& actor);
		virtual ~ParticleComponent();

		virtual bool updatable() const;
		virtual bool renderable() const;

		void setTexture(std::string const& id);
		void setTexture(sf::Texture& texture);
		std::size_t addTextureRect(sf::IntRect const& rect);

		void addAffector(Affector const& affector);
		void clearAffectors();

		virtual void update(sf::Time dt);

		std::size_t getParticleCount() const;
		void clearParticles();

		void enableEmission();
		void disableEmission();

		void setEmissionRate(float particlesPerSecond);
		void setParticleLifetime(Distribution<sf::Time> lifetime);
		void setParticlePosition(Distribution<sf::Vector2f> position);
		void setParticleVelocity(Distribution<sf::Vector2f> velocity);
		void setParticleRotation(Distribution<float> rotation);
		void setParticleRotationSpeed(Distribution<float> rotationSpeed);
		void setParticleScale(Distribution<sf::Vector2f> scale);
		void setParticleColor(Distribution<sf::Color> color);
		void setParticleTextureIndex(Distribution<std::size_t> textureIndex);

		void emitParticle();
		void emitParticles(std::size_t particleAmount);

		void applyWorldGravity();

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
		std::size_t computeParticleCount(sf::Time dt);
		void updateParticle(Particle& particle, sf::Time dt);
		void computeVertices();
		void computeQuads();
		void computeQuad(Quad& quad, sf::IntRect const& rect);

		virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states);

	private:
		std::vector<Particle> mParticles;
		std::vector<Affector> mAffectors;

		sf::Texture* mTexture;
		std::vector<sf::IntRect> mTextureRects;

		sf::VertexArray mVertices;
		bool mNeedsVertexUpdate;
		std::vector<Quad> mQuads;
		bool mNeedsQuadUpdate;

		bool mEmitting;
		float mEmissionRate;
		float mEmissionDifference;

		Distribution<sf::Time> mParticleLifetime;
		Distribution<sf::Vector2f> mParticlePosition;
		Distribution<sf::Vector2f> mParticleVelocity;
		Distribution<float> mParticleRotation;
		Distribution<float> mParticleRotationSpeed;
		Distribution<sf::Vector2f> mParticleScale;
		Distribution<sf::Color> mParticleColor;
		Distribution<std::size_t> mParticleTextureIndex;
};

} // namespace ke

#endif // KE_PARTICLECOMPONENT_HPP
