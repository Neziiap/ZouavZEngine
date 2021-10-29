#include "GameObject.hpp"
#include "System/TimeManager.hpp"
#include "Game/Player.hpp"
#include "Component/RigidStatic.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/MeshRenderer.hpp"
#include "Game/Arrow.hpp"

Arrow::Arrow(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void Arrow::Editor()
{

}

void Arrow::Begin()
{

}

void Arrow::Update()
{
	lifeTime -= TimeManager::GetDeltaTime();
	if (lifeTime <= 0.0f)
	{
		GetGameObject().Destroy();
		return;
	}
	GetGameObject().Translate(direction * speed);
}

void Arrow::Initiate(const Vec3& _direction, float _damage)
{
	direction = _direction;
	damage = _damage;
}

void Arrow::OnTrigger(class Object* _other, class ShapeCollision* _triggerShape)
{
	GameObject* go = dynamic_cast<GameObject*>(_other);
	if (!go)
		GetGameObject().Destroy();
	else
	{
		Player* otherPlayer = go->GetComponent<Player>();
		if (otherPlayer)
		{
			if (otherPlayer->IsAlive())
				otherPlayer->Damage(damage);
		}
	}

}

void Arrow::OnAddComponent()
{
	GetGameObject().AddComponent<RigidStatic>();
	GetGameObject().AddComponent<BoxCollision>()->SetTrigger(true);
	GetGameObject().AddComponent<MeshRenderer>();
}