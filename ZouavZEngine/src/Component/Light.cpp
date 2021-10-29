#include "GameObject.hpp"
#include "Component/Light.hpp"
#include "imgui.h"
#include "Scene.hpp"

Light::Light(GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name)
{
	Scene::GetCurrentScene()->AddLight(this);
}

Light::Light(GameObject* _gameObject, const Vec3 _ambient, const Vec3 _diffuse, const Vec3 _specular, const Vec3 _constLineQuad, const Vec3 _direction, const Vec2 _cutOffOuterCutOff, E_LIGHT_TYPE _type, std::string _name)
	: Component(_gameObject, _name), ambient(_ambient), diffuse(_diffuse), specular(_specular), constLineQuad(_constLineQuad), direction(_direction), cutOffOuterCutOff(_cutOffOuterCutOff), type(_type)
{
	Scene::GetCurrentScene()->AddLight(this);
}

Light::~Light()
{
	toDestroy = true;
	if (Scene::GetCurrentScene())
		Scene::GetCurrentScene()->DeleteLight();
}

void Light::Editor()
{
	ImGui::SliderFloat3("Ambient : ", &ambient.x, 0.0f, 1.0f);
    ImGui::SliderFloat3("Diffuse : ", &diffuse.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Specular : ", &specular.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("ConstLineQuad : ", &constLineQuad.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Direction : ", &direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat2("CutOffOuterCutOff : ", &cutOffOuterCutOff.x, 0.0f, 1.0f);

	std::string oldValue = type == E_LIGHT_TYPE::DIRECTIONAL ? "Directional" : type == E_LIGHT_TYPE::POINT ? "Point" : "Spot";
	if (ImGui::BeginCombo("Type : ", oldValue.c_str()))
	{
		if (ImGui::Selectable("Directional", type == E_LIGHT_TYPE::DIRECTIONAL)) type = E_LIGHT_TYPE::DIRECTIONAL;
		if (ImGui::Selectable("Point", type == E_LIGHT_TYPE::POINT)) type = E_LIGHT_TYPE::POINT;
		if (ImGui::Selectable("Spot", type == E_LIGHT_TYPE::SPOT)) type = E_LIGHT_TYPE::SPOT;
		ImGui::EndCombo();
	}
}

template <class Archive>
static void Light::load_and_construct(Archive& _ar, cereal::construct<Light>& _construct)
{
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
	Vec3 constLineQuad;
	Vec3 direction;
	Vec2 cutOffOuterCutOff;
	int type;
	_ar(ambient.x, ambient.y, ambient.z,
		diffuse.x, diffuse.y, diffuse.z,
		specular.x, specular.y, specular.z,
		constLineQuad.x, constLineQuad.y, constLineQuad.z,
		direction.x, direction.y, direction.z,
		cutOffOuterCutOff.x, cutOffOuterCutOff.y,
		type);

	_construct(GameObject::currentLoadedGameObject, ambient, diffuse, specular, constLineQuad, direction, cutOffOuterCutOff, (E_LIGHT_TYPE)type);
	_ar(cereal::base_class<Component>(_construct.ptr()));
}