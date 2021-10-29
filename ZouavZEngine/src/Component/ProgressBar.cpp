#include "GameObject.hpp"
#include <imgui.h>
#include <algorithm>
#include <glad/glad.h>
#include "System/ResourcesManager.hpp"
#include "Component/ProgressBar.hpp"

ProgressBar::ProgressBar(GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name), shader{*ResourcesManager::GetResource<Shader>("ProgressBar")}
{
    std::vector<Vertex> vertices;
	vertices.push_back(Vertex{ Vec3(0.5, 0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });
	vertices.push_back(Vertex{ Vec3(0.5, -0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });
	vertices.push_back(Vertex{ Vec3(-0.5, -0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });
	vertices.push_back(Vertex{ Vec3(-0.5, 0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });

	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	mesh.InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}

void ProgressBar::Draw()
{
	if (currentValue && maxValue)
	{
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		shader->Use();

		shader->SetMatrix("projection", Mat4::CreateTranslationMatrix(pos)
			* Mat4::CreateScaleMatrix({ size.x, size.y, 1.f }));

		shader->SetVector4("color", backgroundColor);

		glBindVertexArray(mesh.GetID());
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetNbElements(), GL_UNSIGNED_INT, 0);

		shader->SetMatrix("projection", Mat4::CreateTranslationMatrix(Vec3(pos.x - size.x * (1 - (*currentValue / *maxValue)) / 2.0f, pos.y, pos.z))
			* Mat4::CreateScaleMatrix({ (*currentValue / *maxValue) * size.x, size.y, 1.f }));

		shader->SetVector4("color", fillColor);

		glBindVertexArray(mesh.GetID());
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetNbElements(), GL_UNSIGNED_INT, 0);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
}

void ProgressBar::Editor()
{
    ImGui::ColorEdit4("Background Color : ", &backgroundColor.x);
    ImGui::ColorEdit4("Fill Color : ", &fillColor.x);
	ImGui::DragFloat3("Position : ", &pos.x, 0.05f);
	ImGui::DragFloat2("Size : ", &size.x, 0.05f);
}

template <class Archive>
static void ProgressBar::load_and_construct(Archive& _ar, cereal::construct<ProgressBar>& _construct)
{
    _construct(GameObject::currentLoadedGameObject);

	_ar(_construct->fillColor.x, _construct->fillColor.y, _construct->fillColor.z, _construct->fillColor.w,
		_construct->backgroundColor.x, _construct->backgroundColor.y, _construct->backgroundColor.z, _construct->backgroundColor.w,
		_construct->pos.x, _construct->pos.y, _construct->pos.z, _construct->size.x, _construct->size.y, _construct->name);

    _ar(cereal::base_class<Component>(_construct.ptr()));
}