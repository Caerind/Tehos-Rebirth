#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "Sources/Core/Actor.hpp"
#include "Sources/Components/LayerComponent.hpp"

class Terrain : public ke::Actor
{
	public:
		typedef std::shared_ptr<Terrain> Ptr;

		Terrain(ke::Scene& scene, std::size_t id);
		~Terrain();

		void initializeComponents();

		std::size_t getTerrainType() const;

		void loadData();

	private:
		ke::LayerComponent::Ptr mLayer;
		ke::SpriteComponent::Ptr mShadow;
		std::size_t mType;

		class MapObject : public ke::Actor
		{
			public:
				typedef std::shared_ptr<MapObject> Ptr;

				MapObject(ke::Scene& scene);
				~MapObject();

				void initializeComponents();

			private:
				ke::SpriteComponent::Ptr mSprite;
		};
		
};

#endif // TERRAIN_HPP

