#pragma once

#include "Maths/Vec4.hpp"

#include <memory>
 
class Material
{
public:
	std::shared_ptr<class Shader> shader;
	std::shared_ptr<class Texture> texture;
	Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Material();
	Material(std::shared_ptr<class Shader>& _shader, std::shared_ptr<class Texture>& _texture, const Vec4& _color);
	~Material() = default;
};