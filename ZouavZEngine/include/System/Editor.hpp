#pragma once
#include <string>
#include "imgui.h"
#include "ImGuizmo.h"
#include "System/TimeManager.hpp"
#include "Rendering/Camera.hpp"

enum class EDITOR_STATE
{
	EDITING,
	PLAYING,
	PAUSE
};

class Editor
{
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> editorTextures;

	double lastCursorGamePosX = 0;
	double lastCursorGamePosY = 0;

	ImGuizmo::OPERATION currentGizmoOperation = ImGuizmo::TRANSLATE;

	ImGuiStyle* imguiStyle = nullptr;

	EDITOR_STATE state{ EDITOR_STATE::EDITING };

	SceneCamera sceneCamera{ 100, 100 };

	class Clock* editorClock;

	class Engine& engine;

	bool changedScene = true;
	float lastFrameTime = 0.0f;
	float currentTime = 0.0f;
	int n = 0;
	int fps = 0;
public:
	bool isKeyboardEnable;

	Editor(class Engine& _engine);

	void Init();
	void NewFrame();

	const EDITOR_STATE& GetState() const { return state; }

	bool Display(Render& _render);

	void DisplayMainWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayOptionWindow();
	void DisplayMenuBar();
	void DisplaySceneWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayInspector();
	void DisplayConsoleWindow();
	void DisplayGameWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayProject();
	void FileMenu();
	void Update();
	void CameraUpdateRotation();
	void CameraUpdate(float _deltaTime);
	void DisplayHierarchy();
	void DisplayChild(class GameObject* _parent);
	void MoveSelectedGameobject();
	void NewSceneWindow(class Engine& _engine);
	void NewClassWindow();
};