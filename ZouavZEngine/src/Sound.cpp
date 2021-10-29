#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <iostream>
#include <vector>
#include "System/SoundManager.hpp"
#include "System/Debug.hpp"
#include "System/ResourcesManager.hpp"
#include "Sound.hpp"

Sound::Sound(const std::string& _name, const char* _path)
    : Resource(_name, _path)
{
    SF_INFO FileInfos;
    SNDFILE* File = sf_open(_path, SFM_READ, &FileInfos);
    if (!File)
    {
        Debug::LogWarning(std::string(_path).append(" not loaded\n"));
        return;
    }
    ALsizei NbSamples = static_cast<ALsizei>(FileInfos.channels * FileInfos.frames);
    ALsizei SampleRate = static_cast<ALsizei>(FileInfos.samplerate);

    std::vector<ALshort> Samples(NbSamples);
    if (sf_read_short(File, &Samples[0], NbSamples) < NbSamples)
    {
        Debug::LogWarning(std::string(_path).append(" not loaded\n"));
        return;
    }

    sf_close(File);

    ALenum Format = 0;
    switch (FileInfos.channels)
    {
        case 1:  Format = AL_FORMAT_MONO16;   break;
        case 2:  Format = AL_FORMAT_STEREO16; break;
        Debug::LogWarning(std::string(_path).append(" not loaded\n"));
    }

    alGenBuffers(1, &buffer);

    // Remplissage avec les échantillons lus
    alBufferData(buffer, Format, &Samples[0], NbSamples * sizeof(ALushort), SampleRate);

    // Vérification des erreurs
    if (alGetError() != AL_NO_ERROR)
    {
        Debug::LogWarning(std::string(_path).append(" not loaded\n"));
    }
}

Sound::~Sound()
{
    alDeleteBuffers(1, &buffer);
}

void Sound::RemoveFromResourcesManager()
{
    ResourcesManager::RemoveResourceSound(name);
}

void Sound::LinkSource(unsigned int& source)
{
    alSourcei(source, AL_BUFFER, buffer);
}