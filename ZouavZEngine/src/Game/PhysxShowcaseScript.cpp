#include "GameObject.hpp"
#include "System/TimeManager.hpp"
#include <imgui.h>
#include "Game/PhysxShowcaseScript.hpp"

PhysxShowcaseScript::PhysxShowcaseScript(GameObject * _gameobject, std::string _name)
: ScriptComponent(_gameobject, _name)
{}

void PhysxShowcaseScript::Editor()
{
   ScriptComponent::Editor();
   cubePrefab.Editor("Cube prefab : ");
   ImGui::DragFloat("Spawn Cooldown : ", &spawnCooldown, 0.05f);
}

void PhysxShowcaseScript::Begin()
{
   ScriptComponent::Begin();
}

void PhysxShowcaseScript::Update()
{
   ScriptComponent::Update();
   timerSpawn += TimeManager::GetDeltaTime();
   if (timerSpawn >= spawnCooldown)
   {
	   GameObject::Instanciate(*cubePrefab, GetGameObject().WorldPosition());
	   timerSpawn = 0.0f;
   }
}