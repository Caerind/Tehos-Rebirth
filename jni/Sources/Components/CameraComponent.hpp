#ifndef KE_CAMERACOMPONENT_HPP
#define KE_CAMERACOMPONENT_HPP

#include <SFML/Graphics/View.hpp>

#include "../Core/SceneComponent.hpp"

namespace ke
{

class CameraComponent : public SceneComponent
{
    public:
		typedef std::shared_ptr<CameraComponent> Ptr;

		TYPE(CameraComponent)

        CameraComponent(Actor& actor);
		virtual ~CameraComponent();

        sf::View& getView();

		void onTransformNotified();
};

} // namespace ke

#endif // KE_CAMERACOMPONENT_HPP
