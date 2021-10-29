#include "GameObject.hpp"
#include "Game/Arrow.hpp"
#include "Component/Animation.hpp"
#include "Game/EnemyArcher.hpp"

EnemyArcher::EnemyArcher(GameObject * _gameobject, std::string _name)
: Enemy(_gameobject, _name)
{
	detectionDistance = 50.0f;
	distanceToAttack = 15.0f;
	distanceToStop = 15.0f;
}

void EnemyArcher::Editor()
{
	Enemy::Editor();
	arrow.Editor("Arrow Prefab");
}

void EnemyArcher::Begin()
{
	attackAnimName = "Range Zombie Attack.fbx";
	walkAnimName = "Zombie Walk_Range.fbx";
	idleAnimName = "Zombie Idle Range.fbx";
	deathAnimName = "Zombie Death Range.fbx";
	Enemy::Begin();
}

void EnemyArcher::Update()
{
	Enemy::Update();
}

void EnemyArcher::Attack()
{
	if (*arrow)
	{
		Arrow* arrowComp = GameObject::Instanciate(*arrow, GetGameObject().WorldPosition() + Vec3::up * 3.0f)->GetComponent<Arrow>();
		if (arrowComp)
			arrowComp->Initiate((player->WorldPosition() - GetGameObject().WorldPosition() + Vec3::up * -0.5f).Normalized(), attackDamage);
		timerAttackCooldown = 0.0f;
	}
}