#include "System/InputManager.hpp"
#include <GLFW/glfw3.h>
#include "Maths/Vec2.hpp"

#include <iostream>

GLFWwindow* InputManager::window = nullptr;

std::unordered_map<E_MOUSE_BUTTON, std::pair<bool, bool>> InputManager::mouseButtonsState;
std::unordered_map<E_KEYS, std::pair<bool, bool>> InputManager::keysState;
std::initializer_list<E_KEYS> InputManager::allKeys = { E_KEYS::SPACEBAR, E_KEYS::NUM0, E_KEYS::NUM1, E_KEYS::NUM2, E_KEYS::NUM3, E_KEYS::NUM4, E_KEYS::NUM5, E_KEYS::NUM6, E_KEYS::NUM7, E_KEYS::NUM8, E_KEYS::NUM9, E_KEYS::A, E_KEYS::B, E_KEYS::C, E_KEYS::D, E_KEYS::E, E_KEYS::F, E_KEYS::G, E_KEYS::H, E_KEYS::I, E_KEYS::J, E_KEYS::K, E_KEYS::L, E_KEYS::M, E_KEYS::N, E_KEYS::O, E_KEYS::P, E_KEYS::Q, E_KEYS::R, E_KEYS::S, E_KEYS::T, E_KEYS::U, E_KEYS::V, E_KEYS::W, E_KEYS::X, E_KEYS::Y, E_KEYS::Z, E_KEYS::ESCAPE, E_KEYS::TAB, E_KEYS::BACKSPACE, E_KEYS::ARROW_RIGHT, E_KEYS::ARROW_LEFT, E_KEYS::ARROW_DOWN, E_KEYS::ARROW_UP, E_KEYS::F1, E_KEYS::F2, E_KEYS::F3, E_KEYS::F4, E_KEYS::F5, E_KEYS::F6, E_KEYS::F7, E_KEYS::F8, E_KEYS::F9, E_KEYS::F10, E_KEYS::F11, E_KEYS::F12, E_KEYS::LCTRL, E_KEYS::RCTRL, E_KEYS::LSHIFT, E_KEYS::RSHIFT };
bool InputManager::isGameInputActive = false;
Vec2 InputManager::oldMousePos;
Vec2 InputManager::gameOldMousePos;

void InputManager::InitMouseButtons()
{
	for (int i = 0; i < static_cast<int>(E_MOUSE_BUTTON::NUMBER_OF_BUTTONS); i++)
		mouseButtonsState.emplace(static_cast<E_MOUSE_BUTTON>(i), std::make_pair(false, false));
	UpdateOldMousePos();
}

void InputManager::InitKeys()
{
	for (auto key : allKeys)
		keysState.emplace(key, std::make_pair(false, false));
}

void InputManager::UpdateMouseButtons()
{
	for (auto& mouseButton : mouseButtonsState)
	{
		mouseButton.second.second = mouseButton.second.first;
		mouseButton.second.first = glfwGetMouseButton(window, static_cast<int>(mouseButton.first)) == GLFW_PRESS;
	}
}

void InputManager::UpdateKeys()
{
	for (auto& key : keysState)
	{
		key.second.second = key.second.first;
		key.second.first = glfwGetKey(window, static_cast<int>(key.first)) == GLFW_PRESS;
	}
}

void InputManager::UpdateOldMousePos()
{
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	oldMousePos = { (float)cursorX , (float)cursorY };

	if (isGameInputActive)
		gameOldMousePos = oldMousePos;
}

void InputManager::Update()
{
	glfwPollEvents();
	UpdateMouseButtons();
	UpdateKeys();
}

bool InputManager::EditorGetKeyPressed(E_KEYS _key)
{
	return keysState[_key].first;
}

bool InputManager::EditorGetKeyPressedOneTime(E_KEYS _key)
{
	auto& key = keysState[_key];
	return key.first && !key.second;
}

bool InputManager::EditorGetKeyReleased(E_KEYS _key)
{
	return !keysState[_key].first;
}

bool InputManager::EditorGetKeyReleasedOneTime(E_KEYS _key)
{
	auto& key = keysState[_key];
	return !key.first && key.second;
}

bool InputManager::EditorGetMouseButtonPressed(E_MOUSE_BUTTON _button)
{
	return mouseButtonsState[_button].first;
}

bool InputManager::EditorGetMouseButtonPressedOneTime(E_MOUSE_BUTTON _button)
{
	auto& mouseButton = mouseButtonsState[_button];
	return mouseButton.first && !mouseButton.second;
}

bool InputManager::EditorGetMouseButtonReleased(E_MOUSE_BUTTON _button)
{
	return !mouseButtonsState[_button].first;
}

bool InputManager::EditorGetMouseButtonReleasedOneTime(E_MOUSE_BUTTON _button)
{
	auto& mouseButton = mouseButtonsState[_button];
	return !mouseButton.first && mouseButton.second;
}

Vec2 InputManager::EditorGetCursorPos()
{
	double cursorX, cursorY;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	return { (float)cursorX , (float)cursorY };
}

Vec2 InputManager::EditorGetCursorOffsetFromLastFrame()
{
	return EditorGetCursorPos() - oldMousePos;
}

bool InputManager::GetKeyPressed(E_KEYS _key)
{
	return isGameInputActive && keysState[_key].first;
}

bool InputManager::GetKeyPressedOneTime(E_KEYS _key)
{
	auto& key = keysState[_key];
	return isGameInputActive && key.first && !key.second;
}

bool InputManager::GetKeyReleased(E_KEYS _key)
{
	return isGameInputActive && !keysState[_key].first;
}

bool InputManager::GetKeyReleasedOneTime(E_KEYS _key)
{
	auto& key = keysState[_key];
	return isGameInputActive && !key.first && key.second;
}

bool InputManager::GetMouseButtonPressed(E_MOUSE_BUTTON _button)
{
	return isGameInputActive && mouseButtonsState[_button].first;
}

bool InputManager::GetMouseButtonPressedOneTime(E_MOUSE_BUTTON _button)
{
	auto& mouseButton = mouseButtonsState[_button];
	return isGameInputActive && mouseButton.first && !mouseButton.second;
}

bool InputManager::GetMouseButtonReleased(E_MOUSE_BUTTON _button)
{
	return isGameInputActive && !mouseButtonsState[_button].first;
}

bool InputManager::GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON _button)
{
	auto& mouseButton = mouseButtonsState[_button];
	return isGameInputActive && !mouseButton.first && mouseButton.second;
}

Vec2 InputManager::GetCursorPos()
{
	if (isGameInputActive)
	{
		double cursorX, cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		return { (float)cursorX , (float)cursorY };
	}
	else
		return gameOldMousePos;
}

Vec2 InputManager::GetCursorOffsetFromLastFrame()
{
	return GetCursorPos() - gameOldMousePos;
}

void InputManager::SetWindow(GLFWwindow* _window)
{
	window = _window;
}