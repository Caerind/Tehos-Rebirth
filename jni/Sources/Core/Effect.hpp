#ifndef KE_EFFECT_HPP
#define KE_EFFECT_HPP

#include <memory>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "../System/Application.hpp"
#include "../System/Type.hpp"

namespace ke
{

class Scene;
class Effect
{
    public:
		typename std::shared_ptr<Effect> Ptr;

		TYPE(Effect)

		Effect();
        virtual ~Effect();

        static bool isSupported();

        virtual void apply(sf::RenderTexture& texture);

		void setScene(Scene* scene);
		Scene* getScene();

		Log& getLog();
		Application& getApplication();

    protected:
        static void applyShader(sf::Shader const& shader, sf::RenderTarget& output);

		Scene* mScene;
};

class Blur : public Effect
{
	public:
		typename std::shared_ptr<Blur> Ptr;

		TYPE(Blur)

		Blur();

		void apply(sf::RenderTexture& texture);
};

class Pixelate : public Effect
{
	public:
		typename std::shared_ptr<Pixelate> Ptr;

		TYPE(Pixelate)
		
		Pixelate();

		void apply(sf::RenderTexture& texture);
};

} // namespace ke

#endif // KE_EFFECT_HPP
