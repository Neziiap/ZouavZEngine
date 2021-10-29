#include "GameObject.hpp"
#include "Component/Animation.hpp"
#include "Game/Zombie.hpp"

Zombie::Zombie(GameObject * _gameobject, std::string _name)
: Enemy(_gameobject, _name)
{}

void Zombie::Editor()
{
   Enemy::Editor();
}

void Zombie::Begin()
{
   attackAnimName = "Zombie Punching.fbx";
   walkAnimName = "Zombie Walk.fbx";
   idleAnimName = "Zombie Idle.fbx";
   deathAnimName = "Zombie_Death.fbx";
   Enemy::Begin();
}

void Zombie::Update()
{
   Enemy::Update();
}