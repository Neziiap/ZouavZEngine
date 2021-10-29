#include "Component/ScriptComponent.hpp"
#include "GameObject.hpp"
#include "System/ScriptSystem.hpp"

#include "Component/RegisterComponent.hpp"

std::vector<ScriptComponent*> ScriptSystem::scripts;
std::vector<bool (*)(GameObject*)> ScriptSystem::addComponentsFunctions;

void ScriptSystem::Init()
{
	RegisterComponents(addComponentsFunctions);
}

void ScriptSystem::AddScript(ScriptComponent* _script)
{
	scripts.push_back(_script);
}

void ScriptSystem::RemoveScript(ScriptComponent* _script)
{
	for (auto it = scripts.begin(); it != scripts.end(); )
	{
		if (*it == _script)
		{
			scripts.erase(it);
			return;
		}
		else
			++it;
	}
}

void ScriptSystem::Begin()
{
	float scriptsSize = scripts.size();
	for (int i = 0; i < scriptsSize; i++)
	{
		if (scripts[i] && scripts[i]->IsActive())
			scripts[i]->Begin();
	}
}

void ScriptSystem::FixedUpdate()
{
	for (ScriptComponent* script : scripts)
		if (script->IsActive())
			script->FixedUpdate();
}

void ScriptSystem::Update()
{
	float scriptsSize = scripts.size();
	for (int i = 0; i < scriptsSize; i++)
	{
		if (scripts[i] && scripts[i]->IsActive())
			scripts[i]->Update();
	}
}