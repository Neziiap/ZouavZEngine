#include "System/ResourcesManager.hpp"
#include "Rendering/Material.hpp"

Material::Material()
	: shader{ *ResourcesManager::GetResource<Shader>("Default") }, texture{ *ResourcesManager::GetResource<Texture>("Default") }
{}

Material::Material(std::shared_ptr<class Shader>& _shader, std::shared_ptr<class Texture>& _texture, const Vec4& _color)
	: shader{_shader}, texture{_texture}, color{_color}
{}