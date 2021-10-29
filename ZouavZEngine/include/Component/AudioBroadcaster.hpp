#pragma once
#include "Component.hpp"
#include "Sound.hpp"
#include "System/ResourcesManager.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class Vec3;

class AudioBroadcaster : public Component
{
private:
	std::vector<std::shared_ptr<class Sound>> sounds;
	unsigned int source = 0;
	bool ambient = false;
	bool loop = false;
	std::shared_ptr<class Sound> currentSound;

public:
	float volumeIntensity = 1.0f;

	AudioBroadcaster() = delete;
	AudioBroadcaster(class GameObject* _gameObject, std::string _name = "AudioBroadcaster");
	AudioBroadcaster(class GameObject* _gameObject, std::shared_ptr<class Sound>& _sound, std::string _name = "AudioBroadcaster");
	~AudioBroadcaster();

	void Update();
	void Play(std::string _name);
	void Stop();
	
	float GetSoundIntensity();

	void SetPosition(const Vec3& _position);
	void SetVolume(float volume);
	void SetMaxDistance(float _maxDistance);
	void SetLooping(bool _loop);
	void SetAmbient(bool _ambient);

	void Editor() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(ambient, loop, sounds.size());
		for (std::shared_ptr<Sound> sound : sounds)
		{
			_ar(sound->GetName());
			_ar(sound->GetPath());
		}
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<AudioBroadcaster>& _construct);
};


CEREAL_REGISTER_TYPE(AudioBroadcaster)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AudioBroadcaster)
