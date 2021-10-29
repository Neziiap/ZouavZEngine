#pragma once

#include <memory>
#include <vector>

class Clock
{
	friend class TimeManager;
private:

	float time = 0.01f;
	float deltaTime = 0.01677f;
	float previousTime = 0.0f;
	float scale = 1.0f;

	bool activate = true;


public:

	Clock() = default;
	~Clock() = default;

	float GetTime() { return time; }
	float GetDeltaTime() { return deltaTime * scale; }
	float GetUnscaledDeltaTime() { return deltaTime; }

	float SetScale(float _scale) { scale = _scale > 0 ? _scale : -_scale; }

	void Update(float _deltaTime);

	void Activate() { activate = true; }
	void Deactivate() { activate = false; }
	void Reset();

};

class TimeManager
{
	friend class TimeManager;
private:

	static std::vector<std::unique_ptr<Clock>> clocks;

	static float time;
	static float deltaTime;
	static float previousTime;

public:

	static Clock* gameClock;

	static float GetTime() { return gameClock->time; }
	static float GetDeltaTime() { return gameClock->deltaTime * gameClock->scale; }
	static float GetUnscaledDeltaTime() { return gameClock->deltaTime; }

	static float SetScale(float _scale) { gameClock->scale = _scale > 0 ? _scale : -_scale; }
	static Clock* CreateClock();
	static void RemoveClock(Clock* _clock);

	static void Init();

	static void Update();
};