#pragma once

#include "Component.hpp"
#include "Maths/Vec2.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include <memory>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include <cereal/types/base_class.hpp>

class ProgressBar : public Component
{
public:
	ProgressBar() = delete;
	ProgressBar(class GameObject* _gameObject, std::string _name = "ProgressBar");
	
	Mesh mesh{"squareMesh"};
	std::shared_ptr<Shader> shader;

	Vec4 fillColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	Vec4 backgroundColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	Vec3 pos{ 0.0f, 0.0f, 0.0f };
	Vec2 size{ 1.0f, 0.3f };
	float* currentValue{ nullptr };
	float* maxValue{ nullptr };

	void Draw();

	void Editor() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(fillColor.x, fillColor.y, fillColor.z, fillColor.w,
			backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w,
			pos.x, pos.y, pos.z, size.x, size.y, name);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<ProgressBar>& _construct);
};

CEREAL_REGISTER_TYPE(ProgressBar)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ProgressBar)