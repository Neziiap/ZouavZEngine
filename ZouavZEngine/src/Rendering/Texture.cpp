#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>
#include "System/Debug.hpp"
#include "System/ResourcesManager.hpp"
#include "Rendering/Texture.hpp"

const Texture* Texture::errorTexture;

Texture::Texture(const std::string& _name, const char* _path)
    : Resource(_name, _path)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(_path, &width, &height, &nrChannels, 0);

    if (data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        Debug::LogWarning(std::string("Failed to load texture : ").append(_path).append("\n"));

    stbi_image_free(data);
}

void Texture::RemoveFromResourcesManager()
{
    ResourcesManager::RemoveResourceTexture(name);
}

void Texture::Use() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Use(Texture* texture)
{
    if (texture)
        glBindTexture(GL_TEXTURE_2D, texture->textureID);
    else
        glBindTexture(GL_TEXTURE_2D, errorTexture->textureID);
}