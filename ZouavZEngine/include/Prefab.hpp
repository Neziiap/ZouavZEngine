#pragma once
#include <cereal/types/string.hpp>
#include "cereal/access.hpp"
#include "cereal/archives/json.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>


class GameObject;

class Prefab
{
private:
	friend class cereal::access;
	std::string path{"noPath"};
	GameObject* gameobject = nullptr;
public:

	GameObject* operator->()
	{
		return gameobject;
	}
	GameObject* operator*()
	{
		return gameobject;
	}

	void Editor(const char* _label);

	template <class Archive>
	void save(Archive& _ar) const
	{
		_ar(path);
	}
	template <class Archive>
	void load(Archive& _ar)
	{
		_ar(path);
		if (path != "noPath")
			gameobject = GameObject::LoadPrefab(path);
	}

};
