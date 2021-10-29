#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include "Component/AudioBroadcaster.hpp"
#include "System/Debug.hpp"
#include "Maths/Vec3.hpp"
#include "System/SoundManager.hpp"

float SoundManager::mainVolume = 1.0f;
std::vector<AudioBroadcaster*> SoundManager::audioBroadcasters;

void SoundManager::Init()
{
    ALCdevice* Device = alcOpenDevice(nullptr);
    if (!Device)
    {
        Debug::LogWarning("OpenAL Error : Open Device failed");
        return;
    }

    ALCcontext* Context = alcCreateContext(Device, nullptr);
    if (!Context)
    {
        Debug::LogWarning("OpenAL Error : Create Context failed");
        return;
    }

    if (!alcMakeContextCurrent(Context))
    {
        Debug::LogWarning("OpenAL Error : Make Context failed");
        return;
    }

    float values[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, values);
}

void SoundManager::Update()
{
    for (AudioBroadcaster* sound : audioBroadcasters)
    {
        sound->SetVolume(mainVolume * sound->volumeIntensity * sound->GetSoundIntensity());
    }
}

void SoundManager::Destroy()
{
    ALCcontext* Context = alcGetCurrentContext();
    ALCdevice* Device = alcGetContextsDevice(Context);

    alcMakeContextCurrent(nullptr);

    alcDestroyContext(Context);

    alcCloseDevice(Device);
}

void SoundManager::AddSound(AudioBroadcaster* _newSound)
{
    audioBroadcasters.push_back(_newSound);
}

void SoundManager::RemoveSound(AudioBroadcaster* _newSound)
{
    for (auto it = audioBroadcasters.begin(); it != audioBroadcasters.end(); )
    {
        if (*it == _newSound)
        {
            audioBroadcasters.erase(it);
            return;
        }
        else
            ++it;
    }
}

void SoundManager::SetListenerPosition(const Vec3& _position)
{
    alListener3f(AL_POSITION, _position.x, _position.y, _position.z);
}

void SoundManager::SetListenerVelocity(const Vec3& _velocity)
{
    alListener3f(AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
}