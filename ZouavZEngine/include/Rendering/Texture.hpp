#pragma once
#include "Resource.hpp"

class Texture: public Resource
{
    friend class Engine;
private:
    static const Texture* errorTexture;
public:

    unsigned int textureID = 0;
    Texture(const std::string& _name, const char* _path);
    ~Texture() = default;

    void RemoveFromResourcesManager() override;

    static void Use(Texture* _texture);
    void Use() const;
};
