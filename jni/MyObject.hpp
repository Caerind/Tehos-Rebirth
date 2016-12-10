#ifndef MYOBJECT_HPP
#define MYOBJECT_HPP

#include "Sources/Core/Actor.hpp"

class MyObject : public ke::Actor
{
	public:
		typedef std::shared_ptr<MyObject> Ptr;

		TYPE(MyObject)

		MyObject(ke::Scene& scene);

		void initializeComponents();

		void setSize(int x, int y);

		virtual void serialize(ke::Serializer& serializer);
		virtual bool deserialize(ke::Serializer& serializer);

	private:
		ke::ShapeComponent::Ptr mShape;
		ke::LightShapeComponent::Ptr mLight;
		ke::PhysicComponent::Ptr mPhysic;
};

#endif // MYOBJECT_HPP