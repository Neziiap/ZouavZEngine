#pragma once
#include "GameObject.hpp"
#include "System/Terrain.hpp"
#include <vector>

class Scene
{
	friend class Engine;
private:
	static Scene* currentScene;

	Terrain terrain;
	GameObject world{ "World" };

	std::vector<class Light*> lights;

public:

	Scene();
	~Scene();

	static Scene* GetCurrentScene() { return currentScene; }
	GameObject& GetWorld() { return world; }

	static bool NewScene(const std::string& _sceneName, bool _force);

	void Load(bool _changingScene = true);
	void Load(const std::string& _path, bool _changingScene = true);
	void Save();

	void UpdateShaderUniform(const class Camera& _camera);

	void Update();
	void Draw(GameObject* _parent, const class Camera* _camera) const;
	void DrawGUI(GameObject* _parent);
	void SimulatePhyics() const;					//Simulate phyics and update transform of affected gameObjects

	void AddLight(class Light* _newLight);
	void DeleteLight();
	const std::vector<class Light*>& GetLights() { return lights; }

	void DisplayTerrainOptionWindow();
};
