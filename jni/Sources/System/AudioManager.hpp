#ifndef KE_AUDIOMANAGER_HPP
#define KE_AUDIOMANAGER_HPP

#include <map>
#include <memory>
#include <vector>

#include <SFML/Audio.hpp>

namespace ke
{

class AudioManager
{
    public:
        using MusicPtr = std::shared_ptr<sf::Music>;
        using SoundPtr = std::shared_ptr<sf::Sound>;

    public:
        AudioManager();
        ~AudioManager();

        void registerMusicFile(std::string const& id, std::string const& filename);

        MusicPtr playMusic(std::string const& id, bool loop = true, sf::Vector2f const& position = sf::Vector2f());
        void removeMusic(std::string const& id);

        SoundPtr playSound(std::string const& id, sf::Vector2f const& position = sf::Vector2f());

        void play();
        void pause();
        void stop();

        void update();

        void setGlobalVolume(float volume);
        void setMusicVolume(float volume);
        void setSoundVolume(float volume);
        float getGlobalVolume() const;
        float getMusicVolume() const;
        float getSoundVolume() const;

        sf::SoundSource::Status getStatus() const;

    protected:
        std::map<std::string,std::string> mMusicFilenames;
        std::map<std::string,MusicPtr> mMusics;
        std::vector<SoundPtr> mSounds;
        sf::SoundSource::Status mStatus;

        float mMusicVolume;
        float mSoundVolume;
};

} // namespace ke

#endif // KE_AUDIOMANAGER_HPP
