#include "GameObject.hpp"
#include <AL/al.h>
#include "System/SoundManager.hpp"
#include "Component/AudioListener.hpp"
#include "imgui.h"

AudioListener::AudioListener(GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name)
{
	SoundManager::SetListenerVelocity({ 0.0f, 0.0f, 0.0f });
	if (_gameObject)
		SoundManager::SetListenerPosition(_gameObject->WorldPosition());
}

void AudioListener::Update()
{
	SoundManager::SetListenerPosition(GetGameObject().WorldPosition());
}

void AudioListener::Editor()
{
}

template <class Archive>
static void AudioListener::load_and_construct(Archive& _ar, cereal::construct<AudioListener>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	_ar(cereal::base_class<Component>(_construct.ptr()));
}