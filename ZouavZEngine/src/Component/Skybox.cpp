#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/CubemapTexture.hpp"
#include "System/ResourcesManager.hpp"
#include <glad/glad.h>
#include <fstream>
#include "Maths/Vec4.hpp"
#include "Maths/Mat4.hpp"
#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include <imgui.h>
#include "Component/Skybox.hpp"

Skybox::Skybox(class GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name), cubeMesh(*ResourcesManager::GetResource<Mesh>("Default")), shader(*ResourcesManager::GetResource<Shader>("Skybox")), texture(*ResourcesManager::GetResource<CubemapTexture>("Default"))
{}

Skybox::Skybox(class GameObject* _gameObject, std::shared_ptr<CubemapTexture>& _cubemapTexture, std::string _name)
	: Component(_gameObject, _name), cubeMesh(ResourcesManager::GetResource<Mesh>("Default")->get()), shader(ResourcesManager::GetResource<Shader>("Skybox")->get()), texture{ _cubemapTexture }
{}

void Skybox::Draw(const Camera& _camera)
{
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    glUseProgram(shader->id);

    glBindVertexArray(cubeMesh->GetID());
    texture->Use();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Skybox::Editor()
{
    ResourcesManager::ResourceChanger<CubemapTexture>("Texture", texture);
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
        {
            ZASSERT(payload->DataSize == sizeof(std::string), "Error in add new cubemap texture");
            std::string _path = *(const std::string*)payload->Data;
            std::string _truePath = _path;
            size_t start_pos = _truePath.find("\\");
            _truePath.replace(start_pos, 1, "/");

            if (_truePath.find(".dds") != std::string::npos)
            {
                if (texture.use_count() == 2 && texture->IsDeletable())
                    ResourcesManager::RemoveResourceCubemapTexture(texture->GetName());
                texture = *ResourcesManager::AddResourceCubemapTexture(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath.c_str());
            }
        }
        ImGui::EndDragDropTarget();
    }
}

template <class Archive>
static void Skybox::load_and_construct(Archive& _ar, cereal::construct<Skybox>& _construct)
{
    _construct(GameObject::currentLoadedGameObject);
    _ar(cereal::base_class<Component>(_construct.ptr()));
}