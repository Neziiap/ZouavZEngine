#pragma once

#include "Component/Animation.hpp"
#include "GameObject.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "System/Debug.hpp"
#include "imgui.h"
#include "System/TimeManager.hpp"
#include "System/ResourcesManager.hpp"
#include "Component/MeshRenderer.hpp"
#include "Rendering/Texture.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

	
Animation::Animation(GameObject* _gameObject, std::string _animationPath, Mesh* _mesh, std::string _name):
	Component(_gameObject, _name)
{
    currentAnimation = nullptr;
    if (gameObject->GetComponent<MeshRenderer>())
    {
        mesh = gameObject->GetComponent<MeshRenderer>()->mesh.get();
        text = gameObject->GetComponent<MeshRenderer>()->material.texture.get();
    }

    animationShader = *ResourcesManager::GetResource<Shader>("AnimShader");

    finalBonesMatrices.reserve(100 * 16);
    Mat4 mat = Mat4::identity;
    for (int i = 0; i < 100; i++)
    {
        finalBonesMatrices.emplace_back(mat.matrix[0]);
        finalBonesMatrices.emplace_back(mat.matrix[1]);
        finalBonesMatrices.emplace_back(mat.matrix[2]);
        finalBonesMatrices.emplace_back(mat.matrix[3]);
        finalBonesMatrices.emplace_back(mat.matrix[4]);
        finalBonesMatrices.emplace_back(mat.matrix[5]);
        finalBonesMatrices.emplace_back(mat.matrix[6]);
        finalBonesMatrices.emplace_back(mat.matrix[7]);
        finalBonesMatrices.emplace_back(mat.matrix[8]);
        finalBonesMatrices.emplace_back(mat.matrix[9]);
        finalBonesMatrices.emplace_back(mat.matrix[10]);
        finalBonesMatrices.emplace_back(mat.matrix[11]);
        finalBonesMatrices.emplace_back(mat.matrix[12]);
        finalBonesMatrices.emplace_back(mat.matrix[13]);
        finalBonesMatrices.emplace_back(mat.matrix[14]);
        finalBonesMatrices.emplace_back(mat.matrix[15]);
    }
}

void Animation::clearBoneMatrices()
{
    Mat4 mat = Mat4::identity;
    for (int i = 0; i < 100; i++)
    {
        finalBonesMatrices.emplace_back(mat.matrix[0]);
        finalBonesMatrices.emplace_back(mat.matrix[1]);
        finalBonesMatrices.emplace_back(mat.matrix[2]);
        finalBonesMatrices.emplace_back(mat.matrix[3]);
        finalBonesMatrices.emplace_back(mat.matrix[4]);
        finalBonesMatrices.emplace_back(mat.matrix[5]);
        finalBonesMatrices.emplace_back(mat.matrix[6]);
        finalBonesMatrices.emplace_back(mat.matrix[7]);
        finalBonesMatrices.emplace_back(mat.matrix[8]);
        finalBonesMatrices.emplace_back(mat.matrix[9]);
        finalBonesMatrices.emplace_back(mat.matrix[10]);
        finalBonesMatrices.emplace_back(mat.matrix[11]);
        finalBonesMatrices.emplace_back(mat.matrix[12]);
        finalBonesMatrices.emplace_back(mat.matrix[13]);
        finalBonesMatrices.emplace_back(mat.matrix[14]);
        finalBonesMatrices.emplace_back(mat.matrix[15]);
    }
}

void Animation::Editor()
{
    if (ResourcesManager::ResourceChanger<AnimResource>("Animations", currentAnimation))
    {
        if (currentAnimation)
            currentAnimation->UpdateAnimationResources(&rootNode, mesh);
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ProjectFile"))
        {
            ZASSERT(payload->DataSize == sizeof(std::string), "Error in add new animation");

            std::string _path = *(const std::string*)payload->Data;
            std::string _truePath = _path;
            size_t start_pos = _truePath.find("\\");
            _truePath.replace(start_pos, 1, "/");

            if (_truePath.find(".fbx") != std::string::npos || _truePath.find(".dae") != std::string::npos)
            {
                currentAnimation = *ResourcesManager::AddResourceAnimation(_path.substr(_path.find_last_of("/\\") + 1), true, _truePath, mesh);
                if (currentAnimation)
                {
                    currentAnimation->UpdateAnimationResources(&rootNode, mesh);
                    animationsAttached.insert(std::make_pair(currentAnimation->GetName(), currentAnimation));
                    clearBoneMatrices();
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (ResourcesManager::ResourceChanger<AnimResource>("Idle Animation", idleAnimation))
    {
        if (idleAnimation)
            idleAnimation->UpdateAnimationResources(&rootNode, mesh);
    }

    if (currentAnimation)
    {
        ImGui::InputFloat("AnimationDuration", &currentAnimation->animationSpeed);
        ImGui::Checkbox("Loop", &currentAnimation->loop);
        ImGui::Checkbox("GoToIdle", &currentAnimation->goToIdle);
    }
    if (ImGui::Button("Play"))
    {
        play = !play;

        if(currentAnimation)
            currentTime = 0.0f;
    }
}

void Animation::Play(std::string _animName)
{
    currentTime = 0.0f;
    animationFinish = false;
    currentAnimation = animationsAttached.find(_animName)->second;
    rootNode = currentAnimation->rootNode;
    play = true;
}

bool Animation::IsPlaying(std::string _animName)
{
    return currentAnimation->GetName().compare(_animName) == 0;
}

bool Animation::IsFinish(std::string _animName)
{
    return currentAnimation->GetName().compare(_animName) == 0 && IsFinish();
}

void Animation::Draw(const Camera& _camera)
{
    if (currentAnimation)
        currentAnimation->UpdateAnimation(&finalBonesMatrices, &rootNode, TimeManager::GetDeltaTime(), currentAnimation->loop, currentTime, animationFinish);
    else
        return;
    if (IsFinish() && currentAnimation->goToIdle)
        Play(idleAnimation->GetName());

    if(!mesh)
        return;

    animationShader->Use();

    animationShader->SetVector4("color", gameObject->GetComponent<MeshRenderer>() ? gameObject->GetComponent<MeshRenderer>()->material.color : Vec4{1.f, 1.f, 1.f, 1.f});

    animationShader->SetMatrices("finalBonesMatrices", finalBonesMatrices.data(), finalBonesMatrices.size() / 16);
    
    animationShader->SetMatrix("model", Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale()));

    text->Use();
    glBindVertexArray(mesh->GetID());
    glDrawElements(GL_TRIANGLES, mesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}

template <class Archive>
static void Animation::load_and_construct(Archive& _ar, cereal::construct<Animation>& _construct)
{
    int animAttachSize = 0;
    std::vector<std::string> animNames;
    std::vector<std::string> animPaths;
    std::vector<float> animSpeeds;
    std::vector<bool> animLoops;
    std::vector<bool> animGoToIdles;

    _ar(animAttachSize);
    for (int i = 0; i < animAttachSize; i++)
    {
        std::string name;
        std::string path;
        float speed;
        bool loop;
        bool goToIdle;
        _ar(name);
        _ar(path);
        _ar(speed);
        _ar(loop);
        _ar(goToIdle);
        animNames.push_back(name);
        animPaths.push_back(path);
        animSpeeds.push_back(speed);
        animLoops.push_back(loop);
        animGoToIdles.push_back(goToIdle);
    }
    
    std::string idleAnimName;
    _ar(idleAnimName);

    _construct(GameObject::currentLoadedGameObject);
    
    _ar(cereal::base_class<Component>(_construct.ptr()));
    for (int i = 0; i < animAttachSize; i++)
    {
        std::shared_ptr<AnimResource> anim = *ResourcesManager::AddResourceAnimation(animNames[i], true, animPaths[i]);
        anim->animationSpeed = animSpeeds[i];
        anim->loop = animLoops[i];
        anim->goToIdle = animGoToIdles[i];
        MeshRenderer* meshRenderer = _construct->gameObject->GetComponent<MeshRenderer>();

        if (meshRenderer)
        {
            anim->UpdateAnimationResources(&_construct->rootNode, meshRenderer->mesh.get());
        }
        _construct->animationsAttached.insert(std::make_pair(animNames[i], anim));
    }
    
    if(idleAnimName.compare("NoIdle") != 0)
        _construct->idleAnimation = _construct->animationsAttached.find(idleAnimName)->second;
}