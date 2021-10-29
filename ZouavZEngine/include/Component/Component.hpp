#pragma once

#include <vector>
#include <string>
#include <functional>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/access.hpp"
#include <cereal/types/base_class.hpp>

enum class E_COMPONENT
{
	AUDIO_BROADCASTER,
	AUDIO_LISTENER,
	LIGHT,
	MESHRENDERER,
	FONTCOMPONENT,
	BOX_COLLISION,
	CAPSULE_COLLISION,
	SPHERE_COLLISION,
	PLANE,
	RIGID_BODY,
	RIGID_STATIC,
	CAMERA,
	PLAYER,
	GENERATOR,
	SKYBOX,
	ANIMATION,
	NUMBER_OF_COMPONENTS
};

class Component
{
protected:
	friend class Editor;
	friend class GameObject;
	class GameObject* gameObject;
	std::string name = "Component";
	bool isActive = true;

	virtual void InternalActivate() {}
	virtual void InternalDeactivate() {}

public:

	Component() = delete;
	Component(class GameObject* _gameObject, std::string _name = "Component");
	virtual ~Component() = default;

	virtual void Editor() {};

	static bool EditorCollapsingHeader(const char* _name = "Component", std::function<void()> _f = [](){});

	void DeleteFromGameObject();

	bool IsActive() const;
	virtual void Activate() { isActive = true; }
	virtual void Deactivate() { isActive = false; }

	GameObject& GetGameObject() { return *gameObject; }
	const GameObject& GetGameObject() const { return *gameObject; }

	void SetName(std::string _name) { name = _name; }
	std::string GetName() { return name; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(isActive);
		_ar(name);
	}
	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Component>& _construct);
};

CEREAL_REGISTER_TYPE(Component)