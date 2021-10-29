#pragma once

#include <vector>
#include <functional>

class ScriptSystem
{
private:
	friend class Editor;
	static std::vector<class ScriptComponent*> scripts;
	static std::vector<bool (*)(class GameObject*)> addComponentsFunctions;

public:
	ScriptSystem() = delete;
	~ScriptSystem() = delete;

	static void Init();

	static void AddScript(class ScriptComponent* _script);
	static void RemoveScript(class ScriptComponent* _script);

	static void Begin();
	static void FixedUpdate();
	static void Update();
};