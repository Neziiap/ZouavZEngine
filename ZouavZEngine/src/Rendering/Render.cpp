#include "Rendering/Render.hpp"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rendering/Mesh.hpp"
#include "System/Debug.hpp"

Render::Render(int _width, int _height)
    : width(_width), height(_height)
{
      // Init glfw
    ZASSERT(glfwInit(), "Failed to init glfw");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "ZouavZ Engine", nullptr, nullptr);
    ZASSERT(window, "Failed to create glfw window");

    glfwMakeContextCurrent(window);
    ZASSERT(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to load glad");

    glViewport(0, 0, width, height);


    Debug::Log(std::string("GL_VENDOR = ").append(((const char*)glGetString(GL_VENDOR))));
    Debug::Log(std::string("GL_RENDERER = ").append(((const char*)glGetString(GL_RENDERER))));
    Debug::Log(std::string("GL_VERSION = ").append(((const char*)glGetString(GL_VERSION))));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void Render::Init(int _width, int _height)
{

    // Init glfw
    ZASSERT(glfwInit(), "Failed to init glfw");

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    
    width = mode ->width;
    height = mode->height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "ZouavZ Engine", nullptr, nullptr);
    ZASSERT(window, "Failed to create glfw window");

    glfwMakeContextCurrent(window);
    ZASSERT(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to load glad");

    Debug::Log(std::string("GL_VENDOR = ").append(((const char*)glGetString(GL_VENDOR))));
    Debug::Log(std::string("GL_RENDERER = ").append(((const char*)glGetString(GL_RENDERER))));
    Debug::Log(std::string("GL_VERSION = ").append(((const char*)glGetString(GL_VERSION))));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, (int)width / 2, (int)height / 2);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glfwMaximizeWindow(window);

    //init IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mainFramebuffer);
    sceneFramebuffer.Generate(400, 400, GL_RGBA, GL_UNSIGNED_BYTE);
    gameFramebuffer.Generate(400, 400, GL_RGBA, GL_UNSIGNED_BYTE);
}

void Render::Update()
{
    glfwSwapBuffers(window);
}

bool Render::Stop()
{
    return glfwWindowShouldClose(window);
}

void Render::Clear()
{
    ImGui::Render();
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::BindMainFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mainFramebuffer);
    glViewport(0, 0, width, height);
    Clear();
}

void Render::BindSceneFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, sceneFramebuffer.getId());
    glViewport(0, 0, sceneFramebuffer.getWidth(), sceneFramebuffer.getHeight());
    Clear();
}

void Render::BindGameFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, gameFramebuffer.getId());
    glViewport(0, 0, gameFramebuffer.getWidth(), gameFramebuffer.getHeight());
    Clear();
}

void Render::Destroy()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}