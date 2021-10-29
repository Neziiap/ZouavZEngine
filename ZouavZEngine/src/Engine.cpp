#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "System/ResourcesManager.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/Framebuffer.hpp"
#include "Rendering/Font.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/Light.hpp"
#include "Component/AudioBroadcaster.hpp"
#include "Component/AudioListener.hpp"
#include "System/TimeManager.hpp"
#include "System/Terrain.hpp"
#include "System/InputManager.hpp"
#include "System/ScriptSystem.hpp"
#include "System/FontSystem.hpp"
#include "System/Engine.hpp"
#include "System/SoundManager.hpp"
#include "Sound.hpp"
#include <iostream>
#include "System/PhysicSystem.hpp"
#include "Component/BoxCollision.hpp"
#include "Component/SphereCollision.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"

#define EDITOR

Engine::Engine()
    : editor(*this)
{
    render.Init(1300, 800);

    InputManager::SetWindow(render.window);
    InputManager::InitMouseButtons();
    InputManager::InitKeys();

    SoundManager::Init();
    PhysicSystem::Init();
    TimeManager::Init();
    ScriptSystem::Init();


#ifdef EDITOR
    editor.Init();
#endif

    glfwSetInputMode(render.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    double startCursorX, startCursorY;
    glfwGetCursorPos(render.window, &startCursorX, &startCursorY);

    LoadDefaultResources();
}

Engine::~Engine()
{
    //Save();
	render.Destroy();
    SoundManager::Destroy();
    PhysicSystem::Destroy();
}

void Engine::LoadDefaultResources()
{
    ResourcesManager::InitDefaultResources();
    ResourcesManager::AddResourceShader("TerrainShader", false, "resources/TerrainShader.shader");
    ResourcesManager::AddResourceShader("AnimShader", false, "resources/Anim.shader");
    ResourcesManager::AddResourceShader("Font3DShader", false, "resources/Font3DShader.shader");
    ResourcesManager::AddResourceShader("FontBillboardShader", false, "resources/FontBillboardShader.shader");
    ResourcesManager::AddResourceShader("Font2DShader", false, "resources/Font2DShader.shader");
    ResourcesManager::AddResourceShader("GizmosShader", false, "resources/Gizmos.shader");
    Texture::errorTexture = ResourcesManager::AddResourceTexture("Error", false, "resources/error.jpg")->get();
}

void Engine::Load(bool _changedScene)
{
    scene.Load(_changedScene);
}

void Engine::Save()
{
    scene.Save();
}

void Engine::Update()
{
    while (!render.Stop())
    {
        TimeManager::Update();
        InputManager::Update();

        editor.NewFrame();

        //ImGui::ShowDemoWindow();

        if (editor.Display(render))
        {
            scene.DisplayTerrainOptionWindow();
            scene.GetWorld().UpdateTransform(Mat4::identity);
        }

        if (editor.GetState() == EDITOR_STATE::PLAYING)
        {
            ScriptSystem::FixedUpdate();
            scene.Update();
            ScriptSystem::Update();
            SoundManager::Update();
            scene.GetWorld().UpdateTransform(Mat4::identity);
        }
        else
            scene.GetWorld().UpdateTransform(Mat4::identity);

        ////////////////
        render.BindSceneFBO(); 

        scene.UpdateShaderUniform(*SceneCamera::GetSceneCamera());
        scene.Draw(&scene.GetWorld(), SceneCamera::GetSceneCamera());
        scene.terrain.Draw(*SceneCamera::GetSceneCamera());
        FontSystem::DrawFonts(*SceneCamera::GetSceneCamera());

        render.BindGameFBO();
        
        if (Camera::GetMainCamera())
        {
            scene.UpdateShaderUniform(*Camera::GetMainCamera());
            scene.Draw(&scene.GetWorld(), Camera::GetMainCamera());
            scene.terrain.Draw(*SceneCamera::GetSceneCamera());
            FontSystem::DrawFonts(*Camera::GetMainCamera());
            scene.DrawGUI(&scene.GetWorld());
        }
        
        render.BindMainFBO();
        /////////////////
        
        editor.Update();
        render.Update();
        InputManager::UpdateOldMousePos();
        GameObject::DestroyGameObjectIfNeedTo();
    }

    GameObject::prefabGameObjects.clear();
    ResourcesManager::Clear();
    GameObject::gameObjects.clear();
    scene.world.children.clear();
}
