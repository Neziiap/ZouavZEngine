#include "Scene.hpp"
#include "Maths/Mat4.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "Component/ShapeCollision.hpp"
#include "System/PhysicUtils.hpp"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "PxActor.h"
#include "cereal/archives/json.hpp"
#include "Component/ScriptComponent.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include "GameObject.hpp"

bool GameObject::destroyGameObject = false;
bool GameObject::resetScene = false;
GameObject* GameObject::currentLoadedGameObject = nullptr;
std::vector<std::unique_ptr<GameObject>> GameObject::gameObjects;
std::unordered_map < std::string, std::unique_ptr<GameObject >> GameObject::prefabGameObjects;
float GameObject::minY = -1000.0f;

GameObject* GameObject::CreateGameObject(const std::string& _name)
{
	return CreateGameObject(_name, "");
}

GameObject* GameObject::CreateGameObject(const std::string& _name, const std::string& _tag)
{
	gameObjects.emplace_back(std::make_unique<GameObject>(_name, _tag));
	Scene::GetCurrentScene()->GetWorld().AddChild(gameObjects.back().get());
	return gameObjects.back().get();
}

GameObject::GameObject(const std::string& _name, const std::string& _tag)
	: Object(_name, _tag)
{

}

GameObject& GameObject::operator=(const GameObject& _other)
{
	std::stringstream saveFile;
	{
		cereal::JSONOutputArchive oArchive(saveFile);

		_other.save(oArchive);
	}
	{
		cereal::JSONInputArchive iarchive(saveFile);

		(*this).load(iarchive);
	}
	return *this;
}

void GameObject::Destroy() 
{
	toDestroy = true;
	for (GameObject* child : children)
		child->Destroy();
	destroyGameObject = true;
}


void GameObject::Activate() 
{
	isActive = true; 
	for (auto& component : components)
		component->InternalActivate();
}

void GameObject::Deactivate() 
{
	isActive = false; 
	for (auto& component : components)
		component->InternalDeactivate();
}

GameObject* GameObject::Instanciate(GameObject* _gameobject)
{
	GameObject* newGameobject = GameObject::CreateGameObject("");
	*newGameobject = *_gameobject;

	if (_gameobject->parent)
		newGameobject->SetParent(_gameobject->parent);
	else
		newGameobject->SetParent(&Scene::GetCurrentScene()->GetWorld());

	newGameobject->isPrefab = false;
	newGameobject->Activate();

	auto scripts = newGameobject->GetComponents<ScriptComponent>();
	for (auto script : scripts)
		script->Begin();

	return newGameobject;
}

GameObject* GameObject::Instanciate(GameObject* _gameobject, const Vec3& _position)
{
	Vec3 _gameobjectPostion = _gameobject->localPosition;
	_gameobject->localPosition = _position;

	GameObject* newGameobject = GameObject::CreateGameObject("");
	*newGameobject = *_gameobject;

	if (_gameobject->parent)
		newGameobject->SetParent(_gameobject->parent);
	else
		newGameobject->SetParent(&Scene::GetCurrentScene()->GetWorld());
	_gameobject->localPosition = _gameobjectPostion;

	newGameobject->isPrefab = false;
	newGameobject->Activate();

	auto scripts = newGameobject->GetComponents<ScriptComponent>();
	for (auto script : scripts)
		script->Begin();

	return newGameobject;
}

GameObject* GameObject::Instanciate(GameObject* _gameobject, GameObject* _parent)
{
	GameObject* newGameobject = GameObject::CreateGameObject("");
	*newGameobject = *_gameobject;

	if (_parent)
		newGameobject->SetParent(_parent);
	else
		newGameobject->SetParent(&Scene::GetCurrentScene()->GetWorld());

	newGameobject->isPrefab = false;
	newGameobject->Activate();

	auto scripts = newGameobject->GetComponents<ScriptComponent>();
	for (auto script : scripts)
		script->Begin();

	return newGameobject;
}

GameObject* GameObject::Instanciate(GameObject* _gameobject, GameObject* _parent, const Vec3& _position)
{
	Vec3 _gameobjectPostion = _gameobject->localPosition;
	_gameobject->localPosition = _position;

	GameObject* newGameobject = GameObject::CreateGameObject("");
	*newGameobject = *_gameobject;

	if (_parent)
		newGameobject->SetParent(_parent);
	else
		newGameobject->SetParent(&Scene::GetCurrentScene()->GetWorld());
	_gameobject->localPosition = _gameobjectPostion;

	newGameobject->isPrefab = false;
	newGameobject->Activate();

	auto scripts = newGameobject->GetComponents<ScriptComponent>();
	for (auto script : scripts)
		script->Begin();

	return newGameobject;
}

void GameObject::CreatePrefab()
{
	isPrefab = true;
	std::ofstream saveFile;
	saveFile.open(std::string("Project/prefabs/" + name + ".zepref"), std::ios::binary);
	{
		cereal::JSONOutputArchive oArchive(saveFile);

		save(oArchive);
	}
	saveFile.close();
	isPrefab = false;
}

GameObject* GameObject::LoadPrefab(const std::string& _path)
{
	auto emplacePair = prefabGameObjects.emplace(_path, std::make_unique<GameObject>(" ", " "));
	if (!emplacePair.second)
		return emplacePair.first->second.get();
	
	GameObject* newGameObject = emplacePair.first->second.get();

	std::string _truePath = _path;
	size_t start_pos = _truePath.find("\\");
	_truePath.replace(start_pos, 1, "/");

	std::ifstream saveFile;
	saveFile.open(_truePath, std::ios::binary);
	{
		cereal::JSONInputArchive iarchive(saveFile);

		newGameObject->load(iarchive);
	}
	saveFile.close();
	return newGameObject;
}

void GameObject::UpdateTransform(const Mat4& _heritedTransform)
{
	worldPosition = _heritedTransform * localPosition;
	worldRotation = parent ? parent->worldRotation * localRotation : localRotation;
	worldScale = parent ? parent->worldScale * localScale : localScale;

	if (worldPosition.y < minY && parent != nullptr)
		Destroy();

	//update physx transform for simulation
	Rigid* rd = GetComponent<Rigid>();
	if (rd)
		rd->actor->setGlobalPose(PxTransformFromTransformGlobal(static_cast<Transform>(*this)));

	for (GameObject* _child : children)
		_child->UpdateTransform(_heritedTransform * Mat4::CreateTRSMatrix(localPosition, localRotation, localScale)); 
}

bool GameObject::IsChildOf(const GameObject* _gameObject) const
{
	if (!parent)
		return false;

	if (parent == _gameObject)
		return true;

	return parent->IsChildOf(_gameObject);
}

void GameObject::AddChild(GameObject* _child)
{
	children.push_back(_child);
	if (children.back()->parent)
		children.back()->parent->RemoveChild(children.back());
	children.back()->parent = this;
}

void GameObject::SetParent(GameObject* _parent)
{
	if (parent)
		parent->RemoveChild(this);

	parent = _parent;

	if (_parent)
		_parent->children.push_back(this);
}

void GameObject::RemoveChild(GameObject* _child)
{
	for (auto it = children.begin(); it != children.end(); )
	{
		if (*it == _child)
			it = children.erase(it);
		else
			++it;
	}
}

void GameObject::ScriptOnAddComponent()
{
	ScriptComponent* newComponent = dynamic_cast<ScriptComponent*>(components.back().get());
	if (newComponent)
		newComponent->OnAddComponent();
}

const std::vector<std::unique_ptr<Component>>& GameObject::GetComponents()
{
	return components;
}

void GameObject::RemoveComponent(Component* _component)
{
	for (auto component = components.begin(); component != components.end(); component++)
	{
		if (component->get() == _component)
		{
			components.erase(component);
			return;
		}
	}
}

void GameObject::DestroyGameObjectIfNeedTo()
{
	if (destroyGameObject)
	{
		for (auto iter = gameObjects.begin(); iter != gameObjects.end();)
		{
			if (iter->get()->toDestroy)
			{
				iter->get()->SetParent(nullptr);
				for (GameObject* child : iter->get()->children)
					child->SetParent(nullptr);
				iter = gameObjects.erase(iter);
			}
			else
				iter++;
		}
		destroyGameObject = false;
	}
}

GameObject* GameObject::GetGameObjectByName(std::string _name)
{
	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->name == _name)
			return gameObject.get();
	}

	return nullptr;
}

std::vector<GameObject*> GameObject::GetGameObjectsByName(std::string _name)
{
	std::vector<GameObject*> toReturn;

	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->name == _name)
			toReturn.push_back(gameObject.get());
	}

	return toReturn;
}

GameObject* GameObject::GetGameObjectByTag(std::string _tag)
{
	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->tag == _tag)
			return gameObject.get();
	}

	return nullptr;
}

std::vector<GameObject*> GameObject::GetGameObjectsByTag(std::string _tag)
{
	std::vector<GameObject*> toReturn;

	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->tag == _tag)
			toReturn.push_back(gameObject.get());
	}

	return toReturn;
}

void GameObject::OnContact()
{

}