#pragma once

#include "Component/Transform.hpp"
#include "Component/Component.hpp"
#include "Object.hpp"

#include <cereal/types/base_class.hpp>
#include "cereal/types/vector.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include "cereal/access.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class GameObject : public Transform, public Object
{
private:
	friend class Editor;
	friend class Scene;
	friend class Prefab;
	friend class Engine;
	friend class cereal::access;

	static bool destroyGameObject;
	static bool resetScene;
	static std::vector<std::unique_ptr<GameObject>> gameObjects;
	static std::unordered_map<std::string ,std::unique_ptr<GameObject>> prefabGameObjects;
	std::vector<std::unique_ptr<Component>> components;
	std::vector<GameObject*> children;
	GameObject* parent{ nullptr };
	bool isActive{ true };
	bool toDestroy{ false };
	bool isPrefab{ false };
	bool notToSave{ false };
	bool canBeLoad{ false };

	void CreatePrefab();
	static GameObject* LoadPrefab(const std::string& _path);

	void ScriptOnAddComponent();

	static float minY;

public:
	static GameObject* currentLoadedGameObject;

	GameObject() = delete;
	GameObject(const std::string& _name, const std::string& _tag = "");
	GameObject& operator=(const GameObject&);
	~GameObject() = default;

	void Destroy();

	void SetNotToSave(bool _state, bool _canBeLoadAsPrefab = false) { notToSave = _state; canBeLoad = _canBeLoadAsPrefab; }

	bool IsActive() const { return isActive && !isPrefab; }
	void Activate();
	void Deactivate();

	static GameObject* Instanciate(GameObject* _gameobject);
	static GameObject* Instanciate(GameObject* _gameobject, const Vec3& _position);
	static GameObject* Instanciate(GameObject* _gameobject, GameObject* _parent);
	static GameObject* Instanciate(GameObject* _gameobject, GameObject* _parent, const Vec3& _position);

	static GameObject* CreateGameObject(const std::string& _name);
	static GameObject* CreateGameObject(const std::string& _name, const std::string& _tag);

	const std::vector<GameObject*>& GetChildren() const { return children; }
	GameObject& GetChild(int _at) const { return *children[_at]; }
	GameObject& GetParent() const { return *parent; }

	bool IsChildOf(const GameObject* _gameObject) const;
	void AddChild(GameObject* _child);
	void SetParent(GameObject* _parent);
	void RemoveChild(GameObject* _child);

	template<typename T, typename... Args>
	T* AddComponent(Args&&... _args)
	{
		components.emplace_back(std::make_unique<T>(this, _args...));
		ScriptOnAddComponent();
		return static_cast<T*>(components.back().get());
	}

	template<typename T>
	T* GetComponent() const
	{
		for (const std::unique_ptr<Component>& component : components)
		{
			T* returnComponent = dynamic_cast<T*>(component.get());
			if (returnComponent)
				return returnComponent;
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetComponents() const
	{
		std::vector<T*> returnComponents;
		for (const std::unique_ptr<Component>& component : components)
		{
			T* _component = dynamic_cast<T*>(component.get());
			if (_component)
				returnComponents.push_back(_component);
		}
		return returnComponents;
	}

	template<typename T>
	T* GetComponentByName(std::string _name)
	{
		for (const std::unique_ptr<Component>& component : components)
		{
			T* returnComponent = dynamic_cast<T*>(component.get());
			if (returnComponent && _name.compare(returnComponent->GetName()) == 0)
				return returnComponent;
		}
		return nullptr;
	}

	void UpdateTransform(const class Mat4& _heritedTransform);
	const std::vector<std::unique_ptr<Component>>& GetComponents();

	void RemoveComponent(Component* _component);

	virtual void OnContact();

	static void DestroyGameObjectIfNeedTo();
	static GameObject* GetGameObjectByName(std::string _name);
	static std::vector<GameObject*> GetGameObjectsByName(std::string _name);
	static GameObject* GetGameObjectByTag(std::string _tag);
	static std::vector<GameObject*> GetGameObjectsByTag(std::string _tag);

	template <class Archive>
	void load(Archive& _ar)
	{
		int nbChild;

		GameObject* _currentLoadedGameObject = currentLoadedGameObject;
		currentLoadedGameObject = this;

		_ar(name, tag, nbChild, localPosition, localRotation, localScale, isActive, isPrefab);
		UpdateTransform(Mat4::identity);

		_ar(components);
		if (isPrefab)
			currentLoadedGameObject = _currentLoadedGameObject;

		std::string childName;
		std::string childTag;
		int nbChild2;

		for (int i = 0; i < nbChild; ++i)
		{
			GameObject* gameobject;
			_ar(childName, childTag, nbChild2);

			if (isPrefab)
				gameobject = prefabGameObjects.emplace(std::to_string((int)this + i), std::make_unique<GameObject>(childName, childTag)).first->second.get();
			else
				gameobject = CreateGameObject(childName, childTag);

			GameObject* _currentLoadedGameObject = currentLoadedGameObject;
			currentLoadedGameObject = gameobject;

			_ar(gameobject->localPosition, gameobject->localRotation, gameobject->localScale, gameobject->isActive, gameobject->isPrefab);
			UpdateTransform(Mat4::CreateTRSMatrix(WorldPosition(), WorldRotation(), WorldScale()));
			_ar(gameobject->components);

			if (gameobject->isPrefab)
				currentLoadedGameObject = _currentLoadedGameObject;

			loadRecurss(_ar, gameobject, nbChild2);
		}
	}

	template <class Archive>
	void loadRecurss(Archive& _ar, GameObject* _gameobject, int _nbChild)
	{
		std::string childName;
		std::string childTag;
		int nbChild2;

		for (int i = 0; i < _nbChild; ++i)
		{
			_ar(childName, childTag, nbChild2);

			GameObject* gameobject;

			if (isPrefab)
				gameobject = prefabGameObjects.emplace(std::to_string((int)this + i), std::make_unique<GameObject>(childName, childTag)).first->second.get();
			else
				gameobject = CreateGameObject(childName, childTag);

			GameObject* _currentLoadedGameObject = currentLoadedGameObject;
			currentLoadedGameObject = gameobject;

			_ar(gameobject->localPosition, gameobject->localRotation, gameobject->localScale, gameobject->isActive, gameobject->isPrefab);
			UpdateTransform(Mat4::CreateTRSMatrix(_gameobject->WorldPosition(), _gameobject->WorldRotation(), _gameobject->WorldScale()));
			_ar(gameobject->components);

			if (gameobject->isPrefab)
				currentLoadedGameObject = _currentLoadedGameObject;

			_gameobject->AddChild(gameobject);

			loadRecurss(_ar, gameobject, nbChild2);
		}
	}

	template <class Archive>
	void save(Archive& _ar) const
	{
		if (notToSave && !canBeLoad)
			return;
		int nbChild = (int)children.size();
		
		for (GameObject* child : children)
			if (child->notToSave)
				--nbChild;

		_ar(name, tag, nbChild, localPosition, localRotation, localScale, isActive, isPrefab, components);

		for (GameObject* child : children)
		{
			child->isPrefab = isPrefab;
			child->save(_ar);
		}
	}
};
