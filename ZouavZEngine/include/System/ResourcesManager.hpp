#pragma once

#include "Resource.hpp"
#include "Sound.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/CubemapTexture.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Font.hpp"
#include "Rendering/AnimResource.hpp"
#include "Rendering/CubemapTexture.hpp"
#include <unordered_map>
#include <cstdarg>
#include <memory>
#include <string>
#include <iostream>
#include "System/Debug.hpp"

#include "cereal/types/vector.hpp"
#include <cereal/types/string.hpp>
#include "cereal/access.hpp"

class ResourcesManager
{
private:
	static std::unordered_map<std::string, std::shared_ptr<Sound>> soundResources;
	static std::unordered_map<std::string, std::shared_ptr<Mesh>> meshResources;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textureResources;
	static std::unordered_map<std::string, std::shared_ptr<Shader>> shaderResources;
	static std::unordered_map<std::string, std::shared_ptr<Font>> fontResources;
	static std::unordered_map<std::string, std::shared_ptr<CubemapTexture>> cubemapTextureResources;
	static std::unordered_map<std::string, std::shared_ptr<AnimResource>> animationsResources;

public:
	ResourcesManager() = delete;
	
	static void InitDefaultResources();
	static void PrepareClear();
	static void ClearHasToBeDelete();
	static void Clear();

	template <class Archive>
	static void load(Archive& _ar)
	{
		int count;
		std::string name;
		std::string path;
		bool deletable;
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, deletable, path);
			AddResourceSound(name, deletable, path.c_str())->get()->hasToBeDelete = false;
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, deletable, path);
			AddResourceMesh(name, deletable, path.c_str())->get()->hasToBeDelete = false;
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, deletable, path);
			AddResourceTexture(name, deletable, path.c_str())->get()->hasToBeDelete = false;
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, deletable, path);
			AddResourceShader(name, deletable, path.c_str())->get()->hasToBeDelete = false;
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, deletable, path);
			AddResourceFont(name, deletable, path.c_str())->get()->hasToBeDelete = false;
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, deletable, path);
			AddResourceCubemapTexture(name, deletable, path.c_str())->get()->hasToBeDelete = false;
		}
		_ar(count);
		for (int i = 0; i < count; ++i)
		{
			_ar(name, deletable, path);
			AddResourceAnimation(name, deletable, path)->get()->hasToBeDelete = false;
		}
	}

	template <class Archive>
	static void save(Archive& _ar)
	{
		_ar(soundResources.size());
		for (auto& sound : soundResources)
			_ar(sound.first, sound.second->deletable, sound.second.get()->path);
		
		_ar(meshResources.size());
		for (auto& mesh : meshResources)
			_ar(mesh.first, mesh.second->deletable, mesh.second.get()->path);
		
		_ar(textureResources.size());
		for (auto& texture : textureResources)
			_ar(texture.first, texture.second->deletable, texture.second.get()->path);
		
		_ar(shaderResources.size());
		for (auto& shader : shaderResources)
			_ar(shader.first, shader.second->deletable, shader.second.get()->path);

		_ar(fontResources.size());
		for (auto& font : fontResources)
			_ar(font.first, font.second->deletable, font.second.get()->path);

		_ar(cubemapTextureResources.size());
		for (auto& cubemapTexture : cubemapTextureResources)
			_ar(cubemapTexture.first, cubemapTexture.second->deletable, cubemapTexture.second.get()->path);

		_ar(animationsResources.size());
		for (auto& animation : animationsResources)
			_ar(animation.first, animation.second->deletable, animation.second.get()->path);
	}

	template<typename... Args>
	static typename std::shared_ptr<Sound>* AddResourceSound(std::string _name, bool _deletable, Args... _args)
	{
		auto find = soundResources.find(_name);
		if (find != soundResources.end())
			return &find->second;

		auto a = soundResources.emplace(_name, std::make_shared<Sound>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Sound resource : " + _name + " loaded");
			a.first->second->deletable = _deletable;
			return &a.first->second;
		}

		Debug::LogError("Sound resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename std::shared_ptr<Mesh>* AddResourceMesh(std::string _name, bool _deletable, Args... _args)
	{
		auto find = meshResources.find(_name);
		if (find != meshResources.end())
			return &find->second;

		auto a = meshResources.emplace(_name, std::make_shared<Mesh>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Mesh resource : " + _name + " loaded");
			a.first->second->deletable = _deletable;
			return &a.first->second;
		}


		Debug::LogError("Mesh resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename std::shared_ptr<Texture>* AddResourceTexture(std::string _name, bool _deletable, Args... _args)
	{
		auto find = textureResources.find(_name);
		if (find != textureResources.end())
			return &find->second;

		auto a = textureResources.emplace(_name, std::make_shared<Texture>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Texture resource : " + _name + " loaded");
			a.first->second->deletable = _deletable;
			return &a.first->second;
		}

		Debug::LogError("Texture resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename std::shared_ptr<Shader>* AddResourceShader(std::string _name, bool _deletable, Args... _args)
	{
		auto find = shaderResources.find(_name);
		if (find != shaderResources.end())
			return &find->second;

		auto a = shaderResources.emplace(_name, std::make_shared<Shader>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Shader resource : " + _name + " loaded");
			a.first->second->deletable = _deletable;
			return &a.first->second;
		}

		Debug::LogError("Shader resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename std::shared_ptr<Font>* AddResourceFont(std::string _name, bool _deletable, Args... _args)
	{
		auto find = fontResources.find(_name);
		if (find != fontResources.end())
			return &find->second;

		auto a = fontResources.emplace(_name, std::make_shared<Font>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Font resource : " + _name + " loaded");
			a.first->second->deletable = _deletable;
			return &a.first->second;
		}

		Debug::LogError("Font resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename std::shared_ptr<CubemapTexture>* AddResourceCubemapTexture(std::string _name, bool _deletable, Args... _args)
	{
		auto find = cubemapTextureResources.find(_name);
		if (find != cubemapTextureResources.end())
			return &find->second;

		auto a = cubemapTextureResources.emplace(_name, std::make_shared<CubemapTexture>(_name, _args...));
		if (a.second)
		{
			Debug::Log("Cubemap texture resource : " + _name + " loaded");
			a.first->second->deletable = _deletable;
			return &a.first->second;
		}

		Debug::LogError("Cubemap texture resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename... Args>
	static typename std::shared_ptr<AnimResource>* AddResourceAnimation(std::string _name, bool _deletable, Args... _args)
	{
		auto find = animationsResources.find(_name);
		if (find != animationsResources.end())
			return &find->second;

		auto a = animationsResources.emplace(_name, std::make_shared<AnimResource>(_name, _args...));
		if (a.second)
		{
			a.first->second->deletable = _deletable;
			return &a.first->second;
		}

		Debug::LogError("Animation resource : " + _name + " not loaded");
		return nullptr;
	}

	template<typename T>
	static std::shared_ptr<T>* GetResource(std::string _name)
	{
		Debug::LogWarning("Wrong GetResource()");
	}

	template<>
	static std::shared_ptr<Sound>* GetResource<Sound>(std::string _name)
	{
		if (soundResources.find(_name) != soundResources.end())
			return &soundResources.at(_name);
		else
			Debug::LogError("Sound resource : " + _name + " not found");	
		return nullptr;
	}

	template<>
	static std::shared_ptr<Mesh>* GetResource<Mesh>(std::string _name)
	{
		if (meshResources.find(_name) != meshResources.end())
			return &meshResources.at(_name);
		else
			Debug::LogError("Mesh resource : " + _name + " not found");
		return nullptr;
	}

	template<>
	static std::shared_ptr<Texture>* GetResource<Texture>(std::string _name)
	{
		if (textureResources.find(_name) != textureResources.end())
			return &textureResources.at(_name);
		else
			Debug::LogError("Texture resource : " + _name + " not found");
		return nullptr;
	}

	template<>
	static std::shared_ptr<Shader>* GetResource<Shader>(std::string _name)
	{
		if (shaderResources.find(_name) != shaderResources.end())
			return &shaderResources.at(_name);
		else
			Debug::LogError("Shader resource : " + _name + " not found");
		return nullptr;
	}

	template<>
	static std::shared_ptr<Font>* GetResource<Font>(std::string _name)
	{
		if (fontResources.find(_name) != fontResources.end())
			return &fontResources.at(_name);
		else
			Debug::LogError("Font resource : " + _name + " not found");
		return nullptr;
	}

	template<>
	static std::shared_ptr<CubemapTexture>* GetResource<CubemapTexture>(std::string _name)
	{
		if (cubemapTextureResources.find(_name) != cubemapTextureResources.end())
			return &cubemapTextureResources.at(_name);
		else
			Debug::LogError("Cubemap Texture resource : " + _name + " not found");
		return nullptr;
	}

	template<>
	static std::shared_ptr<AnimResource>* GetResource<AnimResource>(std::string _name)
	{
		if (animationsResources.find(_name) != animationsResources.end())
			return &animationsResources.at(_name);
		else
			Debug::LogError("Animation resource : " + _name + " not found");
		
		return nullptr;
	}

	template<typename T>
	static const std::unordered_map<std::string, std::shared_ptr<T>>& GetResources()
	{
		Debug::LogWarning("Wrong GetResources()");
	}

	template<>
	static const std::unordered_map<std::string, std::shared_ptr<Texture>>& GetResources<Texture>()
	{
		return textureResources;
	}

	template<>
	static const std::unordered_map<std::string, std::shared_ptr<Font>>& GetResources<Font>()
	{
		return fontResources;
	}

	template<>
	static const std::unordered_map<std::string, std::shared_ptr<Mesh>>& GetResources<Mesh>()
	{
		return meshResources;
	}

	template<>
	static const std::unordered_map<std::string, std::shared_ptr<Shader>>& GetResources<Shader>()
	{
		return shaderResources;
	}

	template<>
	static const std::unordered_map<std::string, std::shared_ptr<Sound>>& GetResources<Sound>()
	{
		return soundResources;
	}

	template<>
	static const std::unordered_map<std::string, std::shared_ptr<CubemapTexture>>& GetResources<CubemapTexture>()
	{
		return cubemapTextureResources;
	}

	template<>
	static const std::unordered_map<std::string, std::shared_ptr<AnimResource>>& GetResources<AnimResource>()
	{
		return animationsResources;
	}

	static void RemoveResourceSound(std::string _name)
	{
		auto it = soundResources.find(_name);
		if (it != soundResources.end())
			soundResources.erase(it);
	}

	static void RemoveResourceTexture(std::string _name)
	{
		auto it = textureResources.find(_name);
		if (it != textureResources.end())
			textureResources.erase(it);
	}

	static void RemoveResourceFont(std::string _name)
	{
		auto it = fontResources.find(_name);
		if (it != fontResources.end())
			fontResources.erase(it);
	}

	static void RemoveResourceMesh(std::string _name)
	{
		auto it = meshResources.find(_name);
		if (it != meshResources.end())
			meshResources.erase(it);
	}

	static void RemoveResourceShader(std::string _name)
	{
		auto it = shaderResources.find(_name);
		if (it != shaderResources.end())
			shaderResources.erase(_name);
	}
	
	static void RemoveResourceCubemapTexture(std::string _name)
	{
		auto it = cubemapTextureResources.find(_name);
		if (it != cubemapTextureResources.end())
			cubemapTextureResources.erase(it);
	}

	static void RemoveResourceAnimation(std::string _name)
	{
		auto it = animationsResources.find(_name);
		if (it != animationsResources.end())
			animationsResources.erase(it);
	}

	template<typename T>
	static bool ResourceChanger(const char* _label, std::shared_ptr<T>& _resource)
	{
		const std::unordered_map<std::string, std::shared_ptr<Resource>>* resources = reinterpret_cast<const std::unordered_map<std::string, std::shared_ptr<Resource>>*>(&GetResources<T>());
		bool changed = false;
		std::shared_ptr<T> oldResource = _resource;
		ResourceChangerImpl(_label, (std::shared_ptr<Resource>&)_resource, *resources, changed);
		if (changed)
			if (oldResource != _resource && oldResource.use_count() == 2 && oldResource->IsDeletable())
				oldResource->RemoveFromResourcesManager();
		return changed;
	}

	static std::shared_ptr<Resource>& ResourceChangerImpl(const char* _label, std::shared_ptr<Resource>& _resource, const std::unordered_map<std::string, std::shared_ptr<Resource>>& _resources, bool& _changed);
};