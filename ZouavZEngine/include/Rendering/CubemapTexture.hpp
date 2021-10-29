#pragma once
#include "Resource.hpp"

class CubemapTexture : public Resource
{
private:
	unsigned int id = 0;

public:
	CubemapTexture(const std::string& _name, const char* _path);
	~CubemapTexture() = default;
    
	void RemoveFromResourcesManager() override;

	static void Use(CubemapTexture* _texture);
    void Use() const;
};