#include "AudioManager.hpp"
#include "Application.hpp"

namespace ke
{

AudioManager::AudioManager()
{
    mStatus = sf::SoundSource::Playing;
    mMusicVolume = 100.f;
    mSoundVolume = 100.f;
}

AudioManager::~AudioManager()
{
}

void AudioManager::registerMusicFile(std::string const& id, std::string const& filename)
{
    mMusicFilenames[id] = filename;
}

std::shared_ptr<sf::Music> AudioManager::playMusic(std::string const& id, bool loop, sf::Vector2f const& position)
{
    std::string filename;
    if (mMusicFilenames.find(id) != mMusicFilenames.end())
    {
        filename = mMusicFilenames.at(id);
    }
    else
    {
        filename = id;
    }

    auto m = std::make_shared<sf::Music>();
    if (m != nullptr && mStatus != sf::SoundSource::Stopped && mMusics.size() < 16)
    {
        mMusics[id] = m;
        m->openFromFile(filename);
        m->setLoop(loop);
        m->setPosition(position.x, 0.f, position.y);
        m->setVolume(mMusicVolume);
        m->play();
        if (mStatus == sf::SoundSource::Paused)
        {
            m->pause();
        }
        return m;
    }
    return nullptr;
}

void AudioManager::removeMusic(std::string const& id)
{
    for (auto itr = mMusics.begin(); itr != mMusics.end();)
    {
        if (itr->first == id)
        {
            itr = mMusics.erase(itr);
        }
        else
        {
            itr++;
        }
    }
}

std::shared_ptr<sf::Sound> AudioManager::playSound(std::string const& id, sf::Vector2f const& position)
{
    auto s = std::make_shared<sf::Sound>();
    if (s != nullptr && mStatus != sf::SoundSource::Stopped && mSounds.size() < 240)
    {
        mSounds.push_back(s);
        s->setBuffer(Application::getResource<SoundBuffer>(id));
        s->setPosition(position.x, 0.f, position.y);
        s->setVolume(mSoundVolume);
        s->play();
        if (mStatus == sf::SoundSource::Paused)
        {
            s->pause();
        }
        return s;
    }
    return nullptr;
}

void AudioManager::play()
{
    if (mStatus == sf::SoundSource::Paused)
    {
        update();
        for (auto itr = mMusics.begin(); itr != mMusics.end(); itr++)
        {
            itr->second->play();
        }
        for (auto itr = mSounds.begin(); itr != mSounds.end(); itr++)
        {
            (*itr)->play();
        }
        mStatus = sf::SoundSource::Playing;
    }
}

void AudioManager::pause()
{
    if (mStatus == sf::SoundSource::Playing)
    {
        update();
        for (auto itr = mMusics.begin(); itr != mMusics.end(); itr++)
        {
            itr->second->pause();
        }
        for (auto itr = mSounds.begin(); itr != mSounds.end(); itr++)
        {
            (*itr)->pause();
        }
        mStatus = sf::SoundSource::Paused;
    }
}

void AudioManager::stop()
{
    if (mStatus != sf::SoundSource::Stopped)
    {
        mMusics.clear();
        mSounds.clear();
    }
}

void AudioManager::update()
{
    for (auto itr = mMusics.begin(); itr != mMusics.end();)
    {
        if (mStatus != sf::SoundSource::Stopped && itr->second->getStatus() == sf::SoundSource::Stopped)
        {
            itr = mMusics.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    for (auto itr = mSounds.begin(); itr != mSounds.end();)
    {
        if (mStatus != sf::SoundSource::Stopped && (*itr)->getStatus() == sf::SoundSource::Stopped)
        {
            itr = mSounds.erase(itr);
        }
        else
        {
            itr++;
        }
    }
}

void AudioManager::setGlobalVolume(float volume)
{
    sf::Listener::setGlobalVolume(volume);
}

void AudioManager::setMusicVolume(float volume)
{
    mMusicVolume = volume;
    for (auto itr = mMusics.begin(); itr != mMusics.end(); itr++)
    {
        itr->second->setVolume(volume);
    }
}

void AudioManager::setSoundVolume(float volume)
{
    mSoundVolume = volume;
    for (auto itr = mSounds.begin(); itr != mSounds.end(); itr++)
    {
        (*itr)->setVolume(volume);
    }
}

float AudioManager::getGlobalVolume() const
{
	return sf::Listener::getGlobalVolume();
}

float AudioManager::getMusicVolume() const
{
    return mMusicVolume;
}

float AudioManager::getSoundVolume() const
{
    return mSoundVolume;
}

sf::SoundSource::Status AudioManager::getStatus() const
{
    return mStatus;
}

} // namespace ke