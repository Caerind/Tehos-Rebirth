#ifndef KE_NODECOMPONENT_HPP
#define KE_NODECOMPONENT_HPP

#include <SFML/Graphics/CircleShape.hpp>

#include "../Core/SceneComponent.hpp"

namespace ke
{

class NodeComponent : public SceneComponent
{
    public:
		typedef std::shared_ptr<NodeComponent> Ptr;

		TYPE(NodeComponent)

		NodeComponent(Actor& actor);
		virtual ~NodeComponent();

		virtual bool renderable() const;

        void setColor(sf::Color const& color);
        sf::Color getColor() const;

        void setRadius(float radius);
        float getRadius() const;

		virtual void serialize(Serializer& serializer);
		virtual bool deserialize(Serializer& serializer);

	private:
        virtual void renderCurrent(sf::RenderTarget& target, sf::RenderStates states);

    private:
        sf::CircleShape mPoint;
};

} // namespace ke

#endif // KE_NODECOMPONENT_HPP
