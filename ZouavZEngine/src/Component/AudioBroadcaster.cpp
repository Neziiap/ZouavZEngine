#include "GameObject.hpp"
#include "Sound.hpp"
#include "imgui.h"
#include "System/ResourcesManager.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <algorithm>
#include "System/SoundManager.hpp"
#include "Maths/Vec3.hpp"
#include "Component/AudioBroadcaster.hpp"

AudioBroadcaster::AudioBroadcaster(GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name)
{
	alGenSources(1, &source);

	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcef(source, AL_MIN_GAIN, 0.0f);
	alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
	alSourcef(source, AL_REFERENCE_DISTANCE, 4.0f);
	alSourcef(source, AL_MAX_DISTANCE, 60.0f);
	alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	SoundManager::AddSound(this);
}

AudioBroadcaster::AudioBroadcaster(GameObject* _gameObject, std::shared_ptr<Sound>& _sound, std::string _name)
	: Component(_gameObject, _name)
{
	sounds.emplace_back(_sound);

	alGenSources(1, &source);

	alSourcef(source, AL_PITCH, 1.0f);
	alSourcef(source, AL_GAIN, 1.0f);
	alSourcef(source, AL_MIN_GAIN, 0.0f);
	alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
	alSourcef(source, AL_REFERENCE_DISTANCE, 4.0f);
	alSourcef(source, AL_MAX_DISTANCE, 60.0f);
	alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	
	SoundManager::AddSound(this);
}

AudioBroadcaster::~AudioBroadcaster()
{
	SoundManager::RemoveSound(this);
	for (std::shared_ptr<Sound> sound : sounds)
	{
		if (sound.use_count() == 2 && sound->IsDeletable())
			sound->RemoveFromResourcesManager();
	}
	alSourcei(source, AL_BUFFER, 0);
	alDeleteSources(1, &source);
}

void AudioBroadcaster::Update()
{
	for (std::shared_ptr<Sound> sound : sounds)
	{
		if (!ambient && sound)
			SetPosition(GetGameObject().WorldPosition());
	}
}

void AudioBroadcaster::Play(std::string _soundName)
{
	if (IsActive())
	{
		for (std::shared_ptr<Sound> sound : sounds)
		{
			if (sound->GetName().compare(_soundName) == 0)
			{
				currentSound = sound;
				sound->LinkSource(source);
				alSourcePlay(source);
			}
		}
	}
}

void AudioBroadcaster::Stop()
{
	alSourceStop(source);
}

float AudioBroadcaster::GetSoundIntensity()
{
	return currentSound ? currentSound->volumeIntensity : 0.0f;
}

void AudioBroadcaster::SetPosition(const Vec3& _position)
{
	alSource3f(source, AL_POSITION, _position.x, _position.y, _position.z);
}

void AudioBroadcaster::SetVolume(float volume)
{
	alSourcef(source, AL_GAIN, std::max(volume, 0.0f));
}

void AudioBroadcaster::SetMaxDistance(float _maxDistance)
{
	alSourcef(source, AL_MAX_DISTANCE, std::max(_maxDistance, 0.0f));
}

void AudioBroadcaster::SetLooping(bool _loop)
{
	alSourcei(source, AL_LOOPING, _loop);
	loop = _loop;
}

void AudioBroadcaster::SetAmbient(bool _ambient)
{
	alSourcei(source, AL_SOURCE_RELATIVE, !_ambient);
	SetPosition(Vec3::zero);
	ambient = _ambient;
}

void AudioBroadcaster::Editor()
{
	if (ImGui::Button("Add Sound"))
	{
		sounds.emplace_back(nullptr);
	}
	for (std::shared_ptr<Sound>& sound : sounds)
	{
		ResourcesManager::ResourceChanger<Sound>("Sound", sound);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
			{
				ZASSERT(payload->DataSize == sizeof(std::string), "Error in add new texture");
				std::string _path = *(const std::string*)payload->Data;
				std::string _truePath = _path;
				size_t start_pos = _truePath.find("\\");
				_truePath.replace(start_pos, 1, "/");

				if (_truePath.find(".wav") != std::string::npos)
				{
					if (sound.use_count() == 2 && sound->IsDeletable())
						ResourcesManager::RemoveResourceTexture(sound->GetName());
					sound = *ResourcesManager::AddResourceSound(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath.c_str());
				}
			}
			ImGui::EndDragDropTarget();
		}
		if (sound)
			ImGui::DragFloat("Sound volume intensity", &sound->volumeIntensity, 0.01f, 0.0f, 1.0f);
		ImGui::NewLine();
	}
	ImGui::DragFloat("Volume intensity", &volumeIntensity, 0.01f, 0.0f, 1.0f);
	ImGui::Text("Ambient : ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##ambient", &ambient))
		SetAmbient(ambient);

	ImGui::Text("Loop    : ");
	ImGui::SameLine();
	if (ImGui::Checkbox("##loop", &loop))
		SetAmbient(loop);
}

template <class Archive>
static void AudioBroadcaster::load_and_construct(Archive& _ar, cereal::construct<AudioBroadcaster>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	
	std::string soundName;
	std::string soundPath;

	float soundsSize;
	_ar(_construct->ambient, _construct->loop, soundsSize);

	for (int i = 0; i < soundsSize; i++)
	{
		_ar(soundName);
		_ar(soundPath);
		_construct->sounds.emplace_back(*ResourcesManager::AddResourceSound(soundName, true, soundPath.c_str()));
	}

	_ar(cereal::base_class<Component>(_construct.ptr()));
}
