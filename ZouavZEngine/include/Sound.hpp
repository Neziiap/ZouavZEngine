#pragma once
#include "Resource.hpp"
#include "Maths/Vec3.hpp"

class Sound : public Resource
{
private:
	unsigned int buffer = 0;

public:
	float volumeIntensity = 1.0f;

	Sound(const std::string& _name, const char* _path);
	~Sound();

	void RemoveFromResourcesManager() override;

	void LinkSource(unsigned int& sound);
};