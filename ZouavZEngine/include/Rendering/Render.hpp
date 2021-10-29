#pragma once

#include "Rendering/Framebuffer.hpp"

class Framebuffer;

class Render
{
private:
public:
	struct GLFWwindow* window{ nullptr };
	int width{ 0 }, height{ 0 };
	
	int mainFramebuffer = 0;
	Framebuffer gameFramebuffer;
	Framebuffer sceneFramebuffer;

	Render() = default;
	Render(int _width, int _height);
	~Render() = default;

	void Init(int _width, int _height);
	void Update();
	bool Stop();
	void Clear();

	void BindMainFBO();
	void BindSceneFBO();
	void BindGameFBO();

	void Destroy();
};