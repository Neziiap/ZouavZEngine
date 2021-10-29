#pragma once

#include "Maths/Vec2.hpp"

#include <unordered_map>
#include <utility>


enum class E_KEYS
{
	SPACEBAR = 32,
	NUM0 = 48,
	NUM1 = 49,
	NUM2 = 50,
	NUM3 = 51,
	NUM4 = 52,
	NUM5 = 53,
	NUM6 = 54,
	NUM7 = 55,
	NUM8 = 56,
	NUM9 = 57,
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	ESCAPE = 256,
	TAB = 258,
	BACKSPACE = 259,
	ARROW_RIGHT = 262,
	ARROW_LEFT = 263,
	ARROW_DOWN = 264,
	ARROW_UP = 265,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	LCTRL = 341,
	RCTRL = 345,
	LSHIFT = 340,
	RSHIFT = 344,
};

 
enum class E_MOUSE_BUTTON
{
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_MIDDLE,
	BUTTON_4,
	BUTTON_5,
	NUMBER_OF_BUTTONS
};

class InputManager
{
private:
	friend class Editor;

	static bool isGameInputActive;

	static struct GLFWwindow* window;

	static std::unordered_map<E_MOUSE_BUTTON, std::pair<bool, bool>> mouseButtonsState;
	static std::unordered_map<E_KEYS, std::pair<bool, bool>> keysState;

	static std::initializer_list<E_KEYS> allKeys;
	
	InputManager() = delete;
	~InputManager() = delete;

	static bool EditorGetKeyPressed(E_KEYS _key);
	static bool EditorGetKeyPressedOneTime(E_KEYS _key);
	static bool EditorGetKeyReleased(E_KEYS _key);
	static bool EditorGetKeyReleasedOneTime(E_KEYS _key);
	static bool EditorGetMouseButtonPressed(E_MOUSE_BUTTON _button);
	static bool EditorGetMouseButtonPressedOneTime(E_MOUSE_BUTTON _button);
	static bool EditorGetMouseButtonReleased(E_MOUSE_BUTTON _button);
	static bool EditorGetMouseButtonReleasedOneTime(E_MOUSE_BUTTON _button);
	static Vec2 EditorGetCursorPos();
	static Vec2 EditorGetCursorOffsetFromLastFrame();

	static Vec2 oldMousePos; 
	static Vec2 gameOldMousePos;

public:

	static void InitMouseButtons();

	static void InitKeys();

	static void UpdateMouseButtons();
	static void UpdateKeys();
	static void Update();
	static void UpdateOldMousePos();

	static bool GetKeyPressed(E_KEYS _key);
	static bool GetKeyPressedOneTime(E_KEYS _key);
	static bool GetKeyReleased(E_KEYS _key);
	static bool GetKeyReleasedOneTime(E_KEYS _key);
	static bool GetMouseButtonPressed(E_MOUSE_BUTTON _button);
	static bool GetMouseButtonPressedOneTime(E_MOUSE_BUTTON _button);
	static bool GetMouseButtonReleased(E_MOUSE_BUTTON _button);
	static bool GetMouseButtonReleasedOneTime(E_MOUSE_BUTTON _button);

	static Vec2 GetCursorPos();
	static Vec2 GetCursorOffsetFromLastFrame();

	static void SetWindow(struct GLFWwindow* _window);
};