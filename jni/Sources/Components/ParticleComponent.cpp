#include "ParticleComponent.hpp"
#include "../Core/Scene.hpp"

namespace ke
{

ParticleComponent::ParticleComponent(Actor& actor)
	: SceneComponent(actor)
	, mParticles()
	, mAffectors()
	, mTexture(nullptr)
	, mTextureRects()
	, mVertices(sf::Triangles)
	, mNeedsVertexUpdate(true)
	, mQuads()
	, mNeedsQuadUpdate(true)
	, mEmitting(false)
	, mEmissionRate(0.f)
	, mEmissionDifference(0.f)
	, mParticleLifetime(sf::seconds(1.f))
	, mParticlePosition(sf::Vector2f(0.f, 0.f))
	, mParticleVelocity(sf::Vector2f(0.f, 0.f))
	, mParticleRotation(0.f)
	, mParticleRotationSpeed(0.f)
	, mParticleScale(sf::Vector2f(1.f, 1.f))
	, mParticleColor(sf::Color::White)
	, mParticleTextureIndex(0)
{
}

ParticleComponent::~ParticleComponent()
{
	onUnregister();
}

bool ParticleComponent::updatable() const
{
	return true;
}

bool ParticleComponent::renderable() const
{
	return true;
}

void ParticleComponent::setTexture(std::string const& id)
{
	if (getApplication().hasResource(id))
	{
		if (getApplication().isResourceLoaded(id))
		{
			ParticleComponent::setTexture(getApplication().getResource<Texture>(id));
		}
	}
}

void ParticleComponent::setTexture(sf::Texture& texture)
{
	mTexture = &texture;
}

std::size_t ParticleComponent::addTextureRect(sf::IntRect const& rect)
{
	mTextureRects.push_back(rect);
	mNeedsQuadUpdate = true;
	return mTextureRects.size() - 1;
}

void ParticleComponent::addAffector(Affector const& affector)
{
	mAffectors.push_back(affector);
}

void ParticleComponent::clearAffectors()
{
	mAffectors.clear();
}

void ParticleComponent::update(sf::Time dt)
{
	if (mEmitting)
	{
		emitParticles(computeParticleCount(dt));
	}

	mNeedsVertexUpdate = true;

	for (std::size_t i = 0; i < mParticles.size();)
	{
		updateParticle(mParticles[i], dt); // lifetime, move, rotate

		if (mParticles[i].passedLifetime < mParticles[i].totalLifetime)
		{
			for (std::size_t j = 0; j < mAffectors.size(); ++j)
			{
				if (mAffectors[j])
				{
					mAffectors[j](mParticles[i], dt);
				}
			}
			++i;
		}
		else
		{
			mParticles.erase(mParticles.begin() + i);
		}
	}
}

std::size_t ParticleComponent::getParticleCount() const
{
	return mParticles.size();
}

void ParticleComponent::clearParticles()
{
	mParticles.clear();
}

void ParticleComponent::enableEmission()
{
	mEmitting = true;
}

void ParticleComponent::disableEmission()
{
	mEmitting = false;
}

void ParticleComponent::setEmissionRate(float particlesPerSecond)
{
	mEmissionRate = particlesPerSecond;
}

void ParticleComponent::setParticleLifetime(Distribution<sf::Time> lifetime)
{
	mParticleLifetime = lifetime;
}

void ParticleComponent::setParticlePosition(Distribution<sf::Vector2f> position)
{
	mParticlePosition = position;
}

void ParticleComponent::setParticleVelocity(Distribution<sf::Vector2f> velocity)
{
	mParticleVelocity = velocity;
}

void ParticleComponent::setParticleRotation(Distribution<float> rotation)
{
	mParticleRotation = rotation;
}

void ParticleComponent::setParticleRotationSpeed(Distribution<float> rotationSpeed)
{
	mParticleRotationSpeed = rotationSpeed;
}

void ParticleComponent::setParticleScale(Distribution<sf::Vector2f> scale)
{
	mParticleScale = scale;
}

void ParticleComponent::setParticleColor(Distribution<sf::Color> color)
{
	mParticleColor = color;
}

void ParticleComponent::setParticleTextureIndex(Distribution<std::size_t> textureIndex)
{
	mParticleTextureIndex = textureIndex;
}

void ParticleComponent::emitParticle()
{
	Particle particle(mParticleLifetime());
	particle.position = mParticlePosition() + getWorldPosition();
	particle.velocity = mParticleVelocity();
	particle.rotation = mParticleRotation();
	particle.rotationSpeed = mParticleRotationSpeed();
	particle.scale = mParticleScale();
	particle.color = mParticleColor();
	particle.textureIndex = mParticleTextureIndex();
	mParticles.push_back(particle);
}

void ParticleComponent::emitParticles(std::size_t particleAmount)
{
	for (std::size_t i = 0; i < particleAmount; ++i)
	{
		emitParticle();
	}
}

void ParticleComponent::applyWorldGravity()
{
	if (getScene().usePhysic() && getScene().getPhysic() != nullptr)
	{
		mAffectors.push_back([&](Particle& particle, sf::Time dt)
		{
			if (getScene().getPhysic() != nullptr)
			{
				particle.velocity += dt.asSeconds() * getScene().getPhysic()->getGravity();
			}
		});
	}
}

void ParticleComponent::serialize(Serializer& serializer)
{
	SceneComponent::serialize(serializer);
}

bool ParticleComponent::deserialize(Serializer& serializer)
{
	if (!SceneComponent::deserialize(serializer))
	{
		return false;
	}
	return true;
}

std::size_t ParticleComponent::computeParticleCount(sf::Time dt)
{
	// We want to fulfill the desired particle rate as exact as possible. Since the amount of emitted particles per frame is
	// integral, we have to emit sometimes more and sometimes less. mParticleDifference takes care of the deviation each frame.
	float particleAmount = mEmissionRate * dt.asSeconds() + mEmissionDifference;
	std::size_t nbParticles = static_cast<std::size_t>(particleAmount);

	// Compute difference for next frame, return current amount
	mEmissionDifference = particleAmount - nbParticles;
	return nbParticles;
}

void ParticleComponent::updateParticle(Particle& particle, sf::Time dt)
{
	particle.passedLifetime += dt;
	particle.position += dt.asSeconds() * particle.velocity;
	particle.rotation += dt.asSeconds() * particle.rotationSpeed;
}

void ParticleComponent::computeVertices()
{
	mVertices.clear();
	for (std::size_t i = 0; i < mParticles.size(); ++i)
	{
		sf::Transform t;
		t.translate(mParticles[i].position);
		t.rotate(mParticles[i].rotation);
		t.scale(mParticles[i].scale);

		assert(mParticles[i].textureIndex == 0 || mParticles[i].textureIndex < mTextureRects.size());

		const auto& quad = mQuads[mParticles[i].textureIndex];
		for (std::size_t j = 0; j < 6; ++j)
		{
			sf::Vertex v;
			v.position = t.transformPoint(quad[j].position);
			v.texCoords = quad[j].texCoords;
			v.color = mParticles[i].color;

			mVertices.append(v);
		}
	}
}

void ParticleComponent::computeQuads()
{
	assert(mTexture);

	if (mTextureRects.empty())
	{
		mQuads.resize(1);
		computeQuad(mQuads[0], sf::IntRect(0, 0, mTexture->getSize().x, mTexture->getSize().y));
	}
	else
	{
		mQuads.resize(mTextureRects.size());
		for (std::size_t i = 0; i < mTextureRects.size(); ++i)
		{
			computeQuad(mQuads[i], mTextureRects[i]);
		}
	}
}

void ParticleComponent::computeQuad(Quad& quad, sf::IntRect const& tRect)
{
	sf::FloatRect rect(static_cast<sf::FloatRect>(tRect));

	quad[0].texCoords = sf::Vector2f(rect.left, rect.top);
	quad[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
	quad[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
	quad[4].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
	quad[3].texCoords = quad[2].texCoords;
	quad[5].texCoords = quad[0].texCoords;

	quad[0].position = sf::Vector2f(-rect.width, -rect.height) / 2.f;
	quad[1].position = sf::Vector2f(rect.width, -rect.height) / 2.f;
	quad[2].position = sf::Vector2f(rect.width, rect.height) / 2.f;
	quad[4].position = sf::Vector2f(-rect.width, rect.height) / 2.f;
	quad[3].position = quad[2].position;
	quad[5].position = quad[0].position;
}

void ParticleComponent::renderCurrent(sf::RenderTarget & target, sf::RenderStates states)
{
	if (mTexture != nullptr)
	{
		if (mNeedsQuadUpdate)
		{
			computeQuads();
			mNeedsQuadUpdate = false;
		}

		if (mNeedsVertexUpdate)
		{
			computeVertices();
			mNeedsVertexUpdate = false;
		}

		states.texture = mTexture;
		target.draw(mVertices, states);
	}
}

} // namespace ke